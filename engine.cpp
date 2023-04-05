#include "engine.hpp"

string buildSpace = "jknit/";

unsigned long long int systemWaitMS = 0;

void smartSys(const string &Command, ostream &Stream)
{
    Stream << "Calling command `" << Command << "`\n";

    auto start = chrono::high_resolution_clock::now();
    int result = system(Command.c_str());
    auto end = chrono::high_resolution_clock::now();
    unsigned long long int ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    Stream << "Command took " << ns << " ns (" << ns / 1'000'000 << " ms)\n";
    systemWaitMS += ns / 1'000'000;

    if (result != 0)
    {
        Stream << "ERROR: Command exited with code " << result << '\n';
    }
    return;
}

Engine::Engine(const bool &DoLog)
{
    doLog = DoLog;

    if (doLog)
    {
        time_t curTime = time(NULL);
        string logPath = "jknit.log";

        cout << "Logging to '" << logPath << "'\n";

        log.open(logPath);
        if (!log.is_open())
        {
            cout << "Failed to open log!\n";
            doLog = false;
        }
        else
        {
            log << "Time/date: " << ctime(&curTime) << "\n\n";
        }
    }

    return;
}

Engine::~Engine()
{
    if (log.is_open())
    {
        log.close();
    }
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

    for (auto line : latexHeader)
    {
        output << line << '\n';
    }

    string line;
    while (!input.eof())
    {
        getline(input, line);
        if (line == "")
        {
            // This newline makes for prettier .tex docs
            output << '\n';
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
                cout << tags::red_bold
                     << "Error: Invalid code chunk header\n"
                     << tags::reset;

                if (doLog)
                {
                    log << "Error: Invalid code chunk header\n";
                }

                continue;
            }

            string header = line.substr(4, line.size() - 5);

            if (doLog)
            {
                log << "Got header '" << header << "'\n";
            }

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

            if (header == "settings")
            {
                fromString(contents);
                continue;
            }

            // Run replacement
            processChunk(header, contents, output);
        }

        // Math chunk
        else if (line.substr(0, 2) == "$$")
        {
            for (auto l : startMath)
            {
                output << l << '\n';
            }

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

            for (auto l : endMath)
            {
                output << l << '\n';
            }
        }

        // Regular LaTeX
        else
        {
            // For list parsing
            string listChars = "-:;., )";

            /*
            \usepackage[most]{tcolorbox}

            \newtcolorbox{myquote}{colback=yellow!20!white,colframe=yellow!75!black,grow to right by=-10mm,grow to left by=-10mm,
                boxrule=0pt,boxsep=0pt,breakable} \makeatletter

            \newcommand{\blockquote}[1]{  \begin{myquote}  #1  \end{myquote}  }

            %via https://tex.stackexchange.com/questions/310818/blockquotes-in-latex
            */

            // > blockquote
            // \blockquote{}
            if (line[0] == '>')
            {
                output << "\\begin{displayquote}\n"
                       << line.substr(1)
                       << "\n\\end{displayquote}\n";
                continue;
            }

            // --- horizontal line
            // \hrule
            else if (line.substr(0, 2) == "--" || line.substr(0, 2) == "~~" || line.substr(0, 2) == "__" || line.substr(0, 2) == "==")
            {
                output << "\\hrule{}\n";
                continue;
            }

            // # ## ### #### headings
            // \section{} \subsection{} \subsubsection{} etc
            else if (line[0] == '#')
            {
                // # => \section{}
                // ## => \subsection{}
                // ### => \subsubsection{}
                // #### => \paragraph{}
                // #####+ => \subparagraph{}

                int numHashes = 0;
                while (line[numHashes] == '#')
                {
                    numHashes++;
                }

                switch (numHashes)
                {
                case 1:
                    output << "\\section{";
                    break;
                case 2:
                    output << "\\subsection{";
                    break;
                case 3:
                    output << "\\subsubsection{";
                    break;
                case 4:
                    output << "\\paragraph{";
                    break;
                default:
                    output << "\\subparagraph{";
                    break;
                };

                for (auto l : startHeader)
                {
                    output << l << ' ';
                }

                output << line.substr(numHashes) << "}\\hfill\\\n";

                continue;
            }

            // 1. Ordered
            // 2. List
            // \begin{enumerate}
            // \end{enumerate}
            // MUST start with 1, a, or A
            else if (line[0] == '1' || line[0] == 'a' || line[0] == 'A')
            {
                output << "\\begin{enumerate}\n";

                while (line.size() > 0 && listChars.find(line[0]) != string::npos)
                {
                    line.erase(0, 1);
                }
                do
                {
                    output << "\\item " << line << "\n";

                    getline(input, line);
                    while (line.size() > 0 && listChars.find(line[0]) != string::npos)
                    {
                        line.erase(0, 1);
                    }
                } while (line != "");

                output << "\\end{enumerate}\n";

                continue;
            }

            // - Unordered
            // - List
            // \begin{itemize}
            // \end{itemize}
            else if (listChars.find(line[0]) != string::npos)
            {
                output << "\n\\begin{itemize}\n";

                while (line.size() > 0 && listChars.find(line[0]) != string::npos)
                {
                    // Strip beginning
                    while (line.size() > 0 && listChars.find(line[0]) != string::npos)
                    {
                        line.erase(0, 1);
                    }

                    output << "\\item " << line << '\n';

                    getline(input, line);
                }

                output << "\\end{itemize}\n";
                continue;
            }

            // Links [title](https://www.example.com)
            /*
            \usepackage{hyperref}
            \href{link.com}{text}
            */
            else if (line[0] == '[')
            {
                string title, link;

                // Scan until end of title
                int i = 1;
                while (i < line.size() && line[i] != ']')
                {
                    title += line[i];
                    i++;
                }

                // Scan until end of link
                i += 2;
                while (i < line.size() && line[i] != ')')
                {
                    link += line[i];
                    i++;
                }

                // Write latex
                output << "\\href{" << link << "}{" << title << "}\n";

                continue;
            }

            // Image ![alt text](image.jpg){options}
            /*
            \begin{figure}[h]
                \centering
                \includegraphics[width=0.25\textwidth]{name.img}
                \caption{alt text}
            \end{figure}

            %https://www.overleaf.com/learn/latex/Inserting_Images
            */
            else if (line[0] == '!')
            {
                string alt, path, options;

                // Parse caption, path and options
                int i = 2;
                while (i < line.size() && line[i] != ']')
                {
                    alt += line[i];
                    i++;
                }
                i += 2;
                while (i < line.size() && line[i] != ')')
                {
                    path += line[i];
                    i++;
                }
                i += 2;
                while (i < line.size() && line[i] != '}')
                {
                    // Avoid issues with commenting, event if it doesn't *technically* work
                    if (line[i] == '%')
                    {
                        options += "\\textwidth ";
                    }
                    else
                    {
                        options += line[i];
                    }
                    i++;
                }

                // Convert to latex
                output << "\\begin{figure}[h]\n"
                       << "\\centering\n"
                       << "\\includegraphics[" << options << "]{" << path << "}\n"
                       << "\\caption {" << alt << "}\n"
                       << "\\end {figure}\n";

                continue;
            }

            else if (line[0] == '%')
            {
                continue;
            }

            // Iterate through full text
            for (int i = 0; i < line.size(); i++)
            {
                // `code`
                // \verb||
                if (line[i] == '`')
                {
                    output << "\\verb|";

                    i++;
                    while (line[i] != '`')
                    {
                        output << line[i];
                        i++;
                    }
                    output << "|";
                }

                // *italicized*
                // \emph{}
                else if (line[i] == '*')
                {
                    i++;

                    // **bold**
                    // \textbf{}
                    if (line[i] == '*')
                    {
                        output << "\\textbf{";

                        i++;
                        while (line.substr(i, 2) != "**")
                        {
                            output << line[i];
                            i++;
                        }
                        i++;

                        output << "}";
                    }
                    else
                    {
                        output << "\\emph{";

                        while (line[i] != '*')
                        {
                            output << line[i];
                            i++;
                        }

                        output << "}";
                    }
                }

                else
                {
                    output << line[i];
                }
            }

            output << '\n';
        }
    }

    for (auto l : latexFooter)
    {
        output << l << '\n';
    }

    input.close();
    output.close();

    // Clear build folder
    smartSys(rm + buildSpace, log);

    return;
}

map<string, string> parseOptions(const string &What)
{
    map<string, string> out;

    stringstream stream(What);
    string symbol;
    while (!stream.eof())
    {
        stream >> symbol;

        string name, value;
        int i = 0;
        while (i < symbol.size())
        {
            if (symbol[i] == ' ')
            {
                i++;
                continue;
            }
            else if (symbol[i] == '=')
            {
                i++;
                break;
            }
            else
            {
                name += symbol[i];
            }

            i++;
        }
        while (symbol[i] == ' ')
        {
            i++;
        }

        value = symbol.substr(i);

        out[name] = value;
    }

    return out;
}

void Engine::processChunk(const string Header, const string &Contents, ostream &Stream)
{
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

    if (doLog)
    {
        log << "Parsed chunk header. Name: '" << name << "' options: '" << options << "'\n";
    }

    // Safety check
    if (!hasPath(name))
    {
        if (doLog)
        {
            log << "Processing pathless name '" << name << "' (using name as command)\n";
        }

        builders[name] = builder{name};
    }

    // Parse options
    map<string, string> optionsMap = parseOptions(options);

    // Add actual code if wanted
    if (optionsMap.count("echo") == 0 || optionsMap["echo"] == "true")
    {
        for (auto l : startCode)
        {
            Stream << l << '\n';
        }

        Stream << Contents;

        for (auto l : endCode)
        {
            Stream << l << '\n';
        }
    }

    if (optionsMap.count("run") != 0 && optionsMap["run"] == "false")
    {
        return;
    }

    // Construct appropriate temp file name
    string srcfile = buildSpace + name + "_src_" + to_string(time(NULL)) + ".txt";
    string tempfile = buildSpace + name + "_out_" + to_string(time(NULL)) + ".txt";

    // Send our chunk contents to a file
    smartSys(mkdir + buildSpace, log);

    if (doLog)
    {
        log << "Saving chunk source code to file '" << srcfile << "'\n";
    }

    ofstream output(srcfile);
    assert(output.is_open());
    output << Contents;
    output.close();

    // Compile and send output to our temp file
    // Already system-independent, so long as filenames are formatted right
    smartSys(builders[name].commandPath + " " + srcfile + " > " + tempfile, log);

    // Load output temp file into string
    if (doLog)
    {
        log << "Loading output from file '" << tempfile << "'\n";
    }
    string line;
    ifstream input(tempfile);
    assert(input.is_open());

    for (auto l : startOutput)
    {
        Stream << l << '\n';
    }

    while (!input.eof())
    {
        getline(input, line);
        if (line != "")
        {
            Stream << ">> " << line << '\n';
        }
    }

    for (auto l : endOutput)
    {
        Stream << l << '\n';
    }

    input.close();

    if (doLog)
    {
        log << "Done!\n";
    }

    return;
}

void Engine::setPath(const string &Name, const string &Path)
{
    builder toAdd;
    toAdd.commandPath = Path;
    builders[Name] = toAdd;
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
    while (out[0] == '\'' || out[0] == '"' || out[0] == ' ' || out[0] == ':' || out[0] == '=')
    {
        out.erase(0, 1);

        if (out.empty())
        {
            return out;
        }
    }

    while (out.back() == '\'' || out.back() == '"' || out.back() == ' ' || out.back() == ':' || out.back() == '=')
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
    string name, path;

    while (!fromStream.eof())
    {
        name = path = "";
        fromStream >> name >> path;

        if (name == "" || path == "")
        {
            break;
        }

        name = strip(name);
        path = strip(path);

        if (path == "")
        {
            fromStream >> path;
            path = strip(path);
        }

        // Add to list of loaders
        builder toAdd;
        toAdd.commandPath = path;
        builders[name] = toAdd;

        if (doLog)
        {
            log << "Loaded builder '" << name << "' with path '" << path << "'.\n";
            log << "Builder set size: " << builders.size() << '\n';
        }

        fromStream.ignore(128, '\n');
    }

    return;
}

void Engine::toStream(ostream &Stream) const
{
    Stream << toString();
    return;
}
