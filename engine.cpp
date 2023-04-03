#include "engine.hpp"

string buildSpace = "jknit/";

void smartSys(const string &Command)
{
    cout << "Calling command `" << Command << "`\n";
    int result = system(Command.c_str());
    if (result != 0)
    {
        cout << tags::red_bold
             << "Command exited with code " << result << '\n'
             << tags::reset;
    }
    return;
}

Engine::Engine(const string &From)
{
    fromString(From);
    return;
}

Engine::Engine(istream &From)
{
    string toLoad, line;
    while (!From.eof())
    {
        getline(From, line);
        toLoad += line + '\n';
    }

    fromString(toLoad);

    return;
}

// Input: Compatable file
// Output: .tex file
void Engine::processFile(const string &InputFilepath, const string &OutputFilepath)
{
    // Open files
    ifstream input(InputFilepath);
    assert(input.is_open());
    ofstream output(OutputFilepath);
    assert(output.is_open());

    output << latexHeader;

    string line;
    while (!input.eof())
    {
        getline(input, line);
        if (line == "")
        {
            continue;
        }

        // Strip leading whitespace
        while (line[0] == ' ' || line[0] == '\t' || line[0] == '\n')
        {
            line = line.erase(0, 1);
        }

        // Code chunk
        if (line.substr(0, 3) == "```")
        {
            // Get header
            if (line.size() <= 5)
            {
                cout << "ERROR: Invalid code chunk header\n";
                continue;
            }

            string header = line.substr(4, line.size() - 5);
            cout << "Got header '" << header << "'\n";

            // Get contents
            string contents;
            do
            {
                getline(input, line);
                if (line.size() < 3 || line.substr(0, 3) != "```")
                {
                    contents += line + '\n';
                }
            } while (line.size() < 3 || line.substr(0, 3) != "```");

            // Run replacement
            output << "\\begin{verbatim}\n"
                   << processChunk(header, contents)
                   << "\\end{verbatim}\n";
        }

        // Math chunk
        else if (line.substr(0, 2) == "$$")
        {
            output << "\\[\n";

            // Get contents (raw latex)
            do
            {
                getline(input, line);

                // Strip leading whitespace
                while (line[0] == ' ' || line[0] == '\t' || line[0] == '\n')
                {
                    line = line.erase(0, 1);
                }

                if (line.size() < 2 || line.substr(0, 2) != "$$")
                {
                    output << line << '\n';
                }
            } while (line.size() < 2 || line.substr(0, 2) != "$$");

            output << "\\]\n";
        }

        // .md macro scanning
        else
        {
            output << line << '\n';
        }
    }

    output << latexFooter;

    input.close();
    output.close();

    return;
}

string Engine::processChunk(const string Header, const string &Contents)
{
    // Clear build folder
    smartSys("rm -rf " + buildSpace);

    // Parse header to select the correct builder
    string name, options;
    int i;
    for (i = 0; i < Header.size() && Header[i] != ' '; i++)
    {
        name += Header[i];
    }

    if (i + 1 < Header.size())
    {
        options = Header.substr(i + 1);
    }

    cout << "Parsed chunk header. Name: '" << name << "' options: '" << options << "'\n";

    // Safety check
    if (!hasPath(name))
    {
        cout << "Error: No path is defined for '" << name << "'\n";
        throw runtime_error("Error");
        return "";
    }

    // Construct appropriate temp file name
    string srcfile = buildSpace + name + "_src_" + to_string(time(NULL)) + builders[name].ext;
    string tempfile = buildSpace + name + "_out_" + to_string(time(NULL)) + ".txt";

    // Send our chunk contents to a file
    smartSys("mkdir -p " + buildSpace);
    cout << "Saving chunk source code to file '" << srcfile << "'\n";

    ofstream output(srcfile);
    assert(output.is_open());
    output << Contents;
    output.close();

    // Compile and send output to our temp file
    smartSys(builders[name].commandPath + " " + builders[name].options + " " + srcfile + " > " + tempfile);

    // Load output temp file into string
    cout << "Loading output from file '" << tempfile << "'\n";
    string out, line;
    ifstream input(tempfile);
    assert(input.is_open());
    while (!input.eof())
    {
        getline(input, line);
        out += line + '\n';
    }
    input.close();

    cout << "Done!\n";

    // Return loaded text
    return out;
}

void Engine::setPath(const string &Name, const string &Path)
{
    builder toAdd;
    toAdd.commandPath = Path;
    builders[Name] = toAdd;
    return;
}

void Engine::setOptions(const string &Name, const string &Options)
{
    builders[Name].options = Options;
    return;
}

void Engine::appendOptions(const string &Name, const string &Options)
{
    builders[Name].options += Options;
    return;
}

bool Engine::hasPath(const string &What) const
{
    return builders.count(What) != 0;
}

string Engine::toString() const
{
    stringstream out;

    for (auto p : builders)
    {
        out << '"' << p.first << "\": \"" << p.second.commandPath << "\"\n";
    }

    return out.str();
}

string strip(const string &What)
{
    if (What.empty())
    {
        return What;
    }

    string out(What);
    while (out[0] == '\'' || out[0] == '"' || out[0] == ' ' || out[0] == ':')
    {
        out.erase(0, 1);

        if (out.empty())
        {
            return out;
        }
    }

    while (out.back() == '\'' || out.back() == '"' || out.back() == ' ' || out.back() == ':')
    {
        out.erase(out.size() - 1, 1);

        if (out.empty())
        {
            return out;
        }
    }

    return out;
}

void Engine::fromString(const string &From)
{
    assert(!From.empty());

    // Parse loaders
    // "loader name here": "path here" ".ext" "options here"

    stringstream fromStream(From);
    string name, path, ext, options;

    while (!fromStream.eof())
    {
        name = path = ext = options = "";
        fromStream >> name >> path >> ext >> options;

        if (name == "" || path == "")
        {
            break;
        }

        name = strip(name);
        path = strip(path);
        ext = strip(ext);
        options = strip(options);

        // Add to list of loaders
        builder toAdd;
        toAdd.commandPath = path;
        toAdd.ext = ext;
        toAdd.options = options;
        builders[name] = toAdd;

        cout << "Loaded builder '" << name << "' with path '" << path << "'.\n";
        cout << "Builder set size: " << builders.size() << '\n';

        fromStream.ignore(128, '\n');
    }

    return;
}

void Engine::toStream(ostream &Stream) const
{
    Stream << toString();
    return;
}
