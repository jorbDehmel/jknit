#include "engine.hpp"
#include <cctype>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory.h>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>

// Strip quotes off
std::string strip_string(const std::string &_from)
{
    uint64_t offset = 0;

    // While the front and back match and the front is some
    // kind of quote
    while (offset <= _from.size() * 2 &&
           _from[offset] == _from[_from.size() - offset - 1] &&
           (_from[offset] == '"' || _from[offset] == '\''))
    {
        // Increment offset
        ++offset;
    }

    // Construct output w/ given offset
    return _from.substr(offset, _from.size() - offset * 2);
}

std::string strip_header(const std::string &_from)
{
    // Get rid of leading backticks
    uint64_t position = 3;
    std::string out;

    while (position < _from.size() &&
           !std::isalnum(_from[position]))
    {
        ++position;
    }

    out = _from.substr(position);
    position = 0;

    while (position < out.size() && std::isalnum(out[position]))
    {
        ++position;
    }

    out = out.substr(0, position);

    for (uint64_t i = 0; i < out.size(); ++i)
    {
        if (std::isalpha(out[i]))
        {
            out[i] = toupper(out[i]);
        }
    }

    return out;
}

// Takes in a header, including leading backticks
void parse_header(const std::string &_header, Chunk &_into)
{
    // Parse settings from the given header
    /*
     Operator | Purpose
    ----------|-------------
     `*`      | Lone chunk
     `^`      | Hide output
     `~`      | Hide code
    */

    if (_header.find('*') != std::string::npos)
    {
        _into.combine = false;
    }

    if (_header.find('^') != std::string::npos)
    {
        _into.show_output = false;
    }

    if (_header.find('~') != std::string::npos)
    {
        _into.show_code = false;
    }

    // Trim tailing markers
    _into.type = strip_header(_header);
}

// Return a new chunk containing the output of the given code.
Chunk Engine::run_code_chunk(const Builder &_builder,
                             const Chunk &_code)
{
    // Save as file
    Chunk out;
    std::string command;
    const std::string input_file =
        magic_number + "_jknit." + _builder.extension;
    std::ofstream f(input_file);

    if (!f.is_open())
    {
        if (settings.all_errors)
        {
            throw std::runtime_error(
                "Could not write temp files; Check "
                "permissions.");
        }
        else
        {
            std::cerr << "WARNING: "
                      << "Could not write temp files; "
                      << "Check permissions.\n";
            return out;
        }
    }

    if (!settings.log)
    {
        for (const auto &l : _code.lines)
        {
            f << l << '\n';
        }
    }
    else
    {
        log << "Executing:\n```" << _code.type << "\n";
        for (const auto &l : _code.lines)
        {
            log << l << '\n';
            f << l << '\n';
        }
        log << "```\n";
    }
    f.close();

    // Construct command
    command = _builder.commandPath + " " + input_file;

    // Run
    try
    {
        out = run_and_get_output(command);

        // Erase temp file
        std::filesystem::remove_all(input_file);
    }
    catch (...)
    {
        // Erase temp file
        // std::filesystem::remove_all(input_file);

        if (settings.all_errors)
        {
            throw;
        }
        else
        {
            std::cerr << "WARNING: "
                      << "Failed to fetch output of command '"
                      << command << "'\n";
            out.lines.clear();
        }
    }

    // Output chunk
    return out;
}

// Breaks a single output chunk into multiple
std::queue<Chunk> Engine::break_output_chunk(const Chunk &_c)
{
    // 'CHUNK_BREAK' on its own line
    std::queue<Chunk> out;
    Chunk current_chunk = _c;
    current_chunk.lines.clear();

    // Iterate over input lines
    for (const auto &line : _c.lines)
    {
        if (line == "CHUNK_BREAK")
        {
            out.push(current_chunk);
            current_chunk.lines.clear();
        }
        else
        {
            current_chunk.lines.push_back(line);
        }
    }
    out.push(current_chunk);

    return out;
}

// Run the given shell command and get its output.
Chunk Engine::run_and_get_output(const std::string &_cmd)
{
    const static uint64_t buffer_size = 128;
    static char buffer[buffer_size];
    static std::chrono::high_resolution_clock::time_point start,
        stop;
    static uint64_t elapsed_us;

    if (settings.time)
    {
        start = std::chrono::high_resolution_clock::now();
    }

    int result = -1;
    Chunk out;

    out.combine = false;
    out.show_code = out.show_output = true;
    out.type = "OUTPUT";

    memset(buffer, '\0', buffer_size);

    if (settings.log)
    {
        log << "Running w/ cmd `" << _cmd << "`\n";
    }

    FILE *pipe = popen(_cmd.c_str(), "r");
    if (!pipe)
    {
        throw std::runtime_error("Failed to run command '" +
                                 _cmd + "'");
    }

    try
    {
        while (fgets(buffer, sizeof(buffer), pipe))
        {
            out.lines.push_back(buffer);
            while (!out.lines.back().empty() &&
                   out.lines.back().back() == '\n')
            {
                out.lines.back().pop_back();
            }
            memset(buffer, '\0', buffer_size);
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }

    result = pclose(pipe);
    if (result != 0)
    {
        result = errno;
        throw std::runtime_error(
            "Command '" + _cmd +
            "' had non-zero exit code of " +
            std::to_string(result) + ".");
    }

    if (settings.log)
    {
        log << "Yielded output:\n```" << out.type << "\n";
        for (const auto &line : out.lines)
        {
            log << line << '\n';
        }
        log << "```\n";
    }

    if (settings.time)
    {
        stop = std::chrono::high_resolution_clock::now();
        elapsed_us =
            std::chrono::duration_cast<
                std::chrono::microseconds>(stop - start)
                .count();
        external_us += elapsed_us;

        if (settings.log)
        {
            log << "Took " << elapsed_us << " us\n";
        }
    }

    return out;
}

Engine::Engine(const Settings &_s)
{
    settings = _s;
    builders["SETTINGS"] = Builder();

    source.open(settings.source);
    target.open(settings.target);
    if (settings.log)
    {
        log.open("jknit.log");

        if (log.is_open())
        {
            log << "JKnit engine instantiated at " << time(NULL)
                << '\n';
        }
        else if (settings.all_errors)
        {
            throw std::runtime_error(
                "Failed to open log 'jknit.log'");
        }
        else
        {
            std::cerr << "WARNING: "
                      << "Failed to open log 'jknit.log'\n";
            settings.log = false;
        }
    }

    if (!source.is_open())
    {
        throw std::runtime_error("Failed to open source '" +
                                 settings.source + "'");
    }

    if (!target.is_open())
    {
        throw std::runtime_error("Failed to open target '" +
                                 settings.target + "'");
    }
}

Engine::~Engine()
{
    source.close();
    target.close();
    if (settings.log)
    {
        log.close();
    }
}

// Load a file, read each line as settings
void Engine::load_settings_file(const std::string &_filepath)
{
    // Open file
    std::ifstream f(_filepath);
    if (!f.is_open())
    {
        throw std::runtime_error(
            "Failed to open settings file '" + _filepath + "'");
    }

    if (settings.log)
    {
        log << "Loading settings file '" << _filepath << "'\n";
    }

    // Iterate over lines
    std::string line;
    while (!f.eof())
    {
        getline(f, line);
        load_settings_line(line);
    }

    // Close file
    f.close();
}

void Engine::load_settings_line(const std::string &_line)
{
    std::string name, path, print_call, extension;
    std::stringstream from_stream(_line);

    // Get name
    from_stream >> name;

    // Handle path (may be quote-enclosed)
    from_stream >> path;
    if (path[0] == '\'' || path[0] == '"')
    {
        std::string word;
        while (path.back() != path.front())
        {
            from_stream >> word;
            path += ' ' + word;
        }

        path.erase(0, 1);
        path.pop_back();
    }

    // Handle print call (may be quote-enclosed)
    from_stream >> print_call;
    if (print_call[0] == '\'' || print_call[0] == '"')
    {
        std::string word;
        while (print_call.back() != print_call.front())
        {
            from_stream >> word;
            print_call += ' ' + word;
        }

        print_call.erase(0, 1);
        print_call.pop_back();
    }

    // Handle extension
    from_stream >> extension;
    if (extension == "")
    {
        extension = "txt";
    }

    // Process stuff
    name = strip_string(name);
    for (uint64_t i = 0; i < name.size(); ++i)
    {
        if (std::isalpha(name[i]))
        {
            name[i] = toupper(name[i]);
        }
    }

    path = strip_string(path);
    print_call = strip_string(print_call);
    extension = strip_string(extension);

    // Add to list of loaders
    Builder toAdd;
    toAdd.printChunkBreak = print_call;
    toAdd.commandPath = path;
    toAdd.extension = extension;
    builders[name] = toAdd;

    // Log if log is on
    if (settings.log)
    {
        log << "Added builder:\n"
            << "\tName:  '" << name << "'\n"
            << "\tCmd:   '" << path << "'\n"
            << "\tExten: '" << extension << "'\n"
            << "\tPrint: '" << print_call << "'\n";
    }
}

////////////////////////////////////////////////////////////////

RunStats Engine::run()
{
    // Set up stats
    RunStats stats;
    external_us = 0;
    stats.start = std::chrono::high_resolution_clock::now();

    // Parse
    if (settings.log)
    {
        log << "Parsing...\n";
    }
    auto chunks = parse();

    // Construct output
    if (settings.log)
    {
        log << "Passing knitting to derived class...\n";
    }
    knit(chunks);
    if (settings.log)
    {
        log << "Derived class has (presumably) finished.\n";
    }

    // Finalize stats and return
    stats.stop = std::chrono::high_resolution_clock::now();
    stats.external_us = external_us;
    return stats;
}

// Go through the input, extract code from `jmd` to output.
// This creates a list of text/code chunks which should then
// be constructed into output.
std::list<Chunk> Engine::parse()
{
    std::string line, header;
    Chunk current_chunk;
    uint64_t cur_chunk_ws_prefix = 0;
    std::list<Chunk> output;
    uint64_t whitespace_prefix;
    std::map<std::string, Chunk> combined_languages;

    current_chunk.type = "TEXT";

    while (!source.eof())
    {
        getline(source, line);

        whitespace_prefix = 0;
        while (line[whitespace_prefix] == ' ' ||
               line[whitespace_prefix] == '\t')
        {
            ++whitespace_prefix;
        }
        header = line.substr(whitespace_prefix);

        if (strncmp(line.c_str() + whitespace_prefix, "```",
                    3) == 0)
        {
            output.push_back(current_chunk);

            if (current_chunk.type == "TEXT")
            {
                // Beginning a code chunk
                parse_header(header, current_chunk);
                cur_chunk_ws_prefix = whitespace_prefix;

                if (current_chunk.type == "SETTINGS")
                {
                    current_chunk.show_code = false;
                    current_chunk.show_output = false;
                    current_chunk.combine = false;
                }
            }
            else
            {
                // End a code chunk

                // Switch based on combine
                if (current_chunk.combine)
                {
                    const auto lang = current_chunk.type;

                    // Add into existing code for this lang
                    if (builders.count(lang) != 0)
                    {
                        for (const auto &cur_line :
                             current_chunk.lines)
                        {
                            combined_languages[lang]
                                .lines.push_back(cur_line);
                        }

                        combined_languages[lang]
                            .lines.push_back(
                                builders.at(lang)
                                    .printChunkBreak);
                    }
                    else if (settings.all_errors)
                    {
                        throw std::runtime_error(
                            "Invalid language ID '" + lang +
                            "'");
                    }
                    else
                    {
                        std::cerr << "WARNING: "
                                  << "Invalid language ID '"
                                  << lang << "'\n";
                    }
                }

                current_chunk.show_code = true;
                current_chunk.combine = true;
                current_chunk.show_output = true;
                current_chunk.pos_in_type = 0;
                current_chunk.type = "TEXT";
            }

            current_chunk.lines.clear();
        }
        else if (current_chunk.type == "SETTINGS")
        {
            if (settings.log)
            {
                log << "Settings line '" << line << "'\n";
            }
            load_settings_line(line);
        }
        else if (!line.empty())
        {
            if (cur_chunk_ws_prefix <= line.size() &&
                current_chunk.type != "TEXT")
            {
                current_chunk.lines.push_back(
                    line.substr(cur_chunk_ws_prefix));
            }
            else
            {
                current_chunk.lines.push_back(line);
            }
        }
    }
    output.push_back(current_chunk);

    // Build all combined languages
    std::map<std::string, std::queue<Chunk>> combined_output;
    for (const auto &p : combined_languages)
    {
        const auto lang = p.first;
        const auto src = p.second;
        const auto builder = builders.at(lang);

        if (settings.log)
        {
            log << "Building knitted chunk in lang '" << lang
                << "'...\n";
        }

        const auto output = run_code_chunk(builder, src);
        combined_output[lang] = break_output_chunk(output);

        if (settings.log)
        {
            log << "Done.\n";
        }
    }

    // Run any remaining code and break into chunks
    for (auto it = output.begin(); it != output.end(); ++it)
    {
        const auto lang = it->type;

        if (lang == "TEXT" || lang == "SETTINGS")
        {
            // Normal text block; Do nothing
            continue;
        }
        else if (it->combine)
        {
            // Get front from combined output
            if (!combined_output[lang].empty())
            {
                const auto to_insert =
                    combined_output[lang].front();
                combined_output[lang].pop();

                // Insert after this item
                ++it;
                output.insert(it, to_insert);
                --it;
            }
            else if (settings.all_errors)
            {
                throw std::runtime_error(
                    "No remaining output for lang '" + lang +
                    "'");
            }
            else
            {
                std::cerr << "WARNING: "
                          << "No remaining output for lang '"
                          << lang << "'\n";
            }
        }
        else
        {
            // Run as independent chunk now
            if (builders.count(lang) != 0)
            {
                if (settings.log)
                {
                    log << "Building loner chunk in lang '"
                        << lang << "'...\n```\n";
                }

                const auto builder = builders.at(lang);
                const auto to_insert =
                    run_code_chunk(builder, *it);

                if (settings.log)
                {
                    log << "Done.\n";
                }

                // Insert after this item
                ++it;
                output.insert(it, to_insert);
                --it;
            }
            else if (settings.all_errors)
            {
                throw std::runtime_error(
                    "Invalid language ID '" + lang + "'");
            }
            else
            {
                std::cerr << "WARNING: "
                          << "Invalid language ID '" << lang
                          << "'\n";
            }
        }
    }

    // Return built output
    return output;
}
