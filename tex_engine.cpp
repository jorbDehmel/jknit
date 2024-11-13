/*
Jordan Dehmel
2023 - present
*/

#include "tex_engine.hpp"
#include <cstdint>
#include <cwctype>
#include <stack>

void handle_md(const std::list<std::string> &, std::ostream &);

// Translate into latex
void TEXEngine::knit(const std::list<Chunk> &_chunks)
{
    // Header
    for (const auto &l : latexHeader)
    {
        target << l << '\n';
    }

    // Body
    for (const auto &chunk : _chunks)
    {
        if (chunk.lines.empty())
        {
            // Skip empty chunks
            continue;
        }
        else if (chunk.type == "TEXT")
        {
            // Markdown text: This is the hard part.
            handle_md(chunk.lines, target);
        }
        else if (chunk.type == "OUTPUT")
        {
            if (!chunk.show_output)
            {
                continue;
            }

            // Code output
            for (const auto &l : startOutput)
            {
                target << l << '\n';
            }

            for (const auto &l : chunk.lines)
            {
                target << l << '\n';
            }

            for (const auto &l : endOutput)
            {
                target << l << '\n';
            }
        }
        else
        {
            // Code input
            if (!chunk.show_code)
            {
                continue;
            }

            if (lstSupportedLangs.count(chunk.type) != 0)
            {
                target << "\\lstset{language=" << chunk.type
                       << "}\n";
            }
            else
            {
                target << "\\lstset{language=C++}\n";
            }

            for (const auto &l : startCode)
            {
                target << l << '\n';
            }

            for (const auto &l : chunk.lines)
            {
                target << l << '\n';
            }

            for (const auto &l : endCode)
            {
                target << l << '\n';
            }
        }
    }

    // Footer
    for (const auto &l : latexFooter)
    {
        target << l << '\n';
    }
}

////////////////////////////////////////////////////////////////

void TEXEngine::handle_md(const std::list<std::string> &_lines,
                          std::ostream &_target)
{
    const static std::string specialCharacters = "%$~#&^";
    const static std::string listChars = "-:;.,)]";

    std::string line;
    bool in_blockquote = false;
    int64_t ws_offset = -1, prev_ws_offset;
    std::stack<std::pair<int, std::string>> list_closure_stack;

    for (auto it = _lines.begin(); it != _lines.end(); ++it)
    {
        line = *it;
        if (line.empty())
        {
            // Ignore empty lines, even for whitespace counting
            if (in_blockquote)
            {
                _target << "\\end{displayquote}\n";
                in_blockquote = false;
            }
            continue;
        }

        prev_ws_offset = ws_offset;
        ws_offset = 0;

        // Get new whitespace offset
        while (ws_offset < (int64_t)line.size() &&
               iswspace(line[ws_offset]))
        {
            ++ws_offset;
        }

        // Trim off whitespace for further processing
        line = line.substr(ws_offset);

        // Deal with de-indentation within lists
        while (!list_closure_stack.empty() &&
               ws_offset < list_closure_stack.top().first)
        {
            _target << list_closure_stack.top().second;
            list_closure_stack.pop();
        }

        if (line[0] == '%')
        {
            // Ignore comment lines
            continue;
        }

        if (line[0] == '>')
        {
            // Block quote
            if (!in_blockquote)
            {
                _target << "\\begin{displayquote}\n";
                in_blockquote = true;
            }

            handle_md({line.substr(1)}, _target);
            continue;
        }
        else if (in_blockquote)
        {
            _target << "\\end{displayquote}\n";
            in_blockquote = false;
        }

        if (line[0] == '#')
        {
            // Header

            // # => \section{}
            // ## => \subsection{}
            // ### => \subsubsection{}
            // #### => \paragraph{}
            // #####+ => \subparagraph{}

            _target << "\\bigskip{}\n";

            int num_pounds = 0;
            while (line[num_pounds] == '#')
            {
                num_pounds++;
            }

            switch (num_pounds)
            {
            case 1:
                _target << "\\section*{";
                break;
            case 2:
                _target << "\\subsection*{";
                break;
            case 3:
                _target << "\\subsubsection*{";
                break;
            case 4:
                _target << "\\paragraph*{";
                break;
            default:
                _target << "\\subparagraph*{";
                break;
            };

            _target << "\\sffamily{";
            handle_md({line.substr(num_pounds)}, _target);
            _target << "}}~\n";

            _target << "\\bigskip{}\n";
            continue;
        }

        if (line[0] == '[')
        {
            // Link
            std::string title, link;

            // Scan until end of title
            unsigned int i = 1;
            while (i < line.size() && line[i] != ']')
            {
                if (specialCharacters.find(line[i]) !=
                    std::string::npos)
                {
                    title += std::string("\\texttt{") +
                             line[i] + "}";
                }
                else
                {
                    title += line[i];
                }
                i++;
            }

            if (line[i + 1] != '(')
            {
                for (auto c : line)
                {
                    if (specialCharacters.find(c) !=
                        std::string::npos)
                    {
                        _target << "\\texttt{" << c << "}";
                    }
                    else
                    {
                        _target << c;
                    }
                }
                _target << '\n';
                continue;
            }

            // Scan until end of link
            i += 2;
            while (i < line.size() && line[i] != ')')
            {
                if (specialCharacters.find(line[i]) !=
                    std::string::npos)
                {
                    line += std::string("\\texttt{") + line[i] +
                            "}";
                }
                else
                {
                    link += line[i];
                }
                i++;
            }

            // Write latex
            _target << "\\href{" << link << "}{";

            // Catch any internal markdown syntax
            handle_md({title}, _target);

            _target << "}\n";

            continue;
        }

        if (line[0] == '!')
        {
            // Image
            std::string alt, path, options;

            // Parse caption, path and options
            unsigned int i = 2;
            while (i < line.size() && line[i] != ']')
            {
                if (specialCharacters.find(line[i]) !=
                    std::string::npos)
                {
                    alt += std::string("\\texttt{") + line[i] +
                           "}";
                }
                else
                {
                    alt += line[i];
                }
                i++;
            }
            i += 2;
            while (i < line.size() && line[i] != ')')
            {
                if (specialCharacters.find(line[i]) !=
                    std::string::npos)
                {
                    path += std::string("\\texttt{") + line[i] +
                            "}";
                }
                else
                {
                    path += line[i];
                }
                i++;
            }
            i += 2;
            while (i < line.size() && line[i] != '}')
            {
                // Percentage parsing
                if (line[i] >= '0' && line[i] <= '9')
                {
                    std::string num = "00";
                    while (i < line.size() && line[i] >= '0' &&
                           line[i] <= '9')
                    {
                        num += line[i];
                        i++;
                    }

                    if (i < line.size() && line[i] == '%')
                    {
                        num = num.substr(0, num.size() - 2) +
                              "." + num.substr(num.size() - 2);
                        options += num + "\\textwidth ";
                        i++;
                    }
                    else
                    {
                        options += num;
                    }
                }

                // Avoid issues with commenting
                if (i < line.size() && line[i] != '%' &&
                    line[i] != '}')
                {
                    options += line[i];
                }
                i++;
            }

            if (options == "")
            {
                options = "width=0.5\\textwidth";
            }

            // Convert to latex
            _target << "\\begin{figure}[h]\n"
                    << "\\centering\n"
                    << "\\includegraphics[" << options << "]{"
                    << path << "}\n"
                    << "\\caption {";

            // Catches any markdown syntax in captions
            handle_md({alt}, _target);

            _target << "}\n"
                    << "\\end {figure}\n";

            continue;
        }

        if (line.starts_with("--") || line.starts_with("~~") ||
            line.starts_with("___") || line.starts_with("=="))
        {
            // Horizontal rule
            _target << "\\hrule{}\n";
            continue;
        }

        if (listChars.find(line[0]) != std::string::npos)
        {
            // Unnumbered list
            if (list_closure_stack.empty() ||
                ws_offset > prev_ws_offset)
            {
                // New sublist
                _target << "\\begin{itemize}\n";
                list_closure_stack.push(
                    {ws_offset, "\\end{itemize}\n"});
            }
            _target << "\\item ";

            // Write the rest of this line
            handle_md({line.substr(2)}, _target);
            continue;
        }

        if (line.size() > 1 && isalnum(line[0]) &&
            listChars.find(line[1]) != std::string::npos)
        {
            // Numbered list
            if (list_closure_stack.empty() ||
                ws_offset > prev_ws_offset)
            {
                // New sublist
                _target << "\\begin{enumerate}\n";
                list_closure_stack.push(
                    {ws_offset, "\\end{enumerate}\n"});
            }
            _target << "\\item ";

            // Write the rest of this line
            handle_md({line.substr(2)}, _target);
            continue;
        }

        // Normal text line
        for (uint64_t i = 0; i < line.size(); ++i)
        {
            const char c = line[i];

            switch (c)
            {
            case '*':
            case '_':
                ++i;
                if (line[i] == c)
                {
                    // Boldface
                    _target << "\\textbf{";
                    for (++i;
                         i + 1 < line.size() &&
                         !(line[i] == c && line[i + 1] == c);
                         ++i)
                    {
                        _target << line[i];
                    }
                    ++i;
                    _target << '}';
                }
                else
                {
                    // Italics
                    _target << "\\textit{";
                    for (; i + 1 < line.size() && line[i] != c;
                         ++i)
                    {
                        _target << line[i];
                    }
                    _target << '}';
                }

                break;

            case '$':
                _target << c;
                for (++i; line[i] != c; ++i)
                {
                    _target << line[i];
                }
                _target << c;
                break;

            case '`':
                _target << "\\texttt{";
                for (++i; line[i] != c; ++i)
                {
                    _target << line[i];
                }
                _target << '}';
                break;

            // Otherwise uncovered TeX-illegal characters
            case '%':
            case '~':
            case '#':
            case '^':
                _target << '\\' << c;
                break;

            // Base case
            default:
                _target << c;
                break;
            }
        }
        _target << '\n';

        continue;
    }

    while (!list_closure_stack.empty())
    {
        _target << list_closure_stack.top().second;
        list_closure_stack.pop();
    }
}
