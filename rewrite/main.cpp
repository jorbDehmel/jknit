/*
JKnit CLI
2023-present
Jordan Dehmel
*/

#include "engine.hpp"
#include "md_engine.hpp"
#include "tex_engine.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <list>
#include <string>

void load_engine(
    Engine &_e,
    const std::list<std::string> &_settings_files)
{
    for (const auto &f : _settings_files)
    {
        _e.load_settings_file(f);
    }

    // Default settings
    _e.load_settings_line(
        "py /bin/python3 'print(\"CHUNK_BREAK\")' py");
    _e.load_settings_line(
        "python /bin/python3 'print(\"CHUNK_BREAK\")' py");
    _e.load_settings_line(
        "octave octave 'printf(\"CHUNK_BREAK\\n\");' m");
    _e.load_settings_line(
        "bash /usr/bin/sh 'echo CHUNK_BREAK' sh");
    _e.load_settings_line(
        "js node 'console.log('CHUNK_BREAK');' js");
}

int main(int c, char *v[])
{
    Settings settings;
    std::list<std::string> settings_files;
    RunStats stats;
    bool target_tex = false;
    settings.log = settings.time = settings.all_errors = false;
    settings.source = "";
    settings.target = "a.md";

    // Parse CLI input
    std::string arg;
    for (uint64_t cur_arg = 1; cur_arg < c; ++cur_arg)
    {
        arg = v[cur_arg];

        // Handle normal flag
        if (arg.front() == '-')
        {
            for (uint64_t i = 1; i < arg.size(); ++i)
            {
                switch (arg[i])
                {
                case 't': // Time
                case 'T':
                    settings.time = !settings.time;
                    break;
                case 'l': // Log
                case 'L':
                    settings.log = !settings.log;
                    break;
                case 'o': // Set target
                case 'O':
                    ++cur_arg;
                    if (cur_arg >= c)
                    {
                        std::cerr << "'-o' must not be last "
                                  << "arg.\n";
                        return 1;
                    }
                    settings.target = v[cur_arg];
                    break;
                case 'f': // Settings file
                case 'F':
                    ++cur_arg;
                    if (cur_arg >= c)
                    {
                        std::cerr << "'-f' must not be last "
                                  << "arg.\n";
                        return 1;
                    }
                    settings_files.push_back(v[cur_arg]);
                    break;
                case 'v': // Version
                case 'V':
                    std::cout << "JKnit version "
                              << VERSION << '\n'
                              << "2023-present, GPLv3\n";
                    break;
                case 'e': // Warnings to errors
                case 'E':
                    settings.all_errors = !settings.all_errors;
                    break;
                case 'x': // Force tex mode
                case 'X':
                    target_tex = !target_tex;
                    break;
                case 'h': // Help
                case 'H':
                    std::cout
                            << "JKnit version " << VERSION
                            << '\n'
                            << "Live RMD-style markdown "
                            << "documents\n\n"
                            << "CLI flags:\n"
                            << "-e Warnings to errors\n"
                            << "-h Help (this)\n"
                            << "-l Toggle log (default off)\n"
                            << "-t Toggle timer (default off)\n"
                            << "-v Version\n"
                            << "-x Force TeX mode\n"
                            << '\n'
                            << "Jordan Dehmel, 2023 - present\n"
                            << "GPLv3\n";
                    break;
                default:
                    std::cerr << "Unrecognized flag '" << arg[i]
                            << "'\n";
                    break;
                }
            }
        }

        // Default case; Set source
        else
        {
            settings.source = arg;
        }
    }

    // Generate loader object
    // Run engine and save to file
    if (target_tex)
    {
        TEXEngine e(settings);
        load_engine(e, settings_files);
        stats = e.run();
    }
    else
    {
        MDEngine e(settings);
        load_engine(e, settings_files);
        stats = e.run();
    }

    if (settings.time)
    {
        const auto total_us = std::chrono::duration_cast<
                    std::chrono::microseconds>(
                        stats.stop - stats.start).count();
        const auto jknit_us = total_us - stats.external_us;
        const double percent_jknit = 100.0 *
                                     (double)(jknit_us) /
                                     (double)(total_us);
        const double percent_extern = 100.0 - percent_jknit;

        std::cout << "Total us:                   "
                  << total_us << '\n'
                  << "JKnit-attributable us:      "
                  << jknit_us << '\n'
                  << "Non-JKnit us:               "
                  << total_us - jknit_us << '\n'
                  << "Percent JKnit-attributable: "
                  << percent_jknit << '\n'
                  << "Percent Non-JKnit:          "
                  << percent_extern << '\n';
    }

    return 0;
}
