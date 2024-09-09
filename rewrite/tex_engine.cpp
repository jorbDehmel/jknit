/*
Jordan Dehmel
2023 - present
GPLv3
*/

#include "tex_engine.hpp"
#include <cwctype>

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
                target << "\\lstset{language="
                       << chunk.type
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

void handle_md(
    const std::list<std::string> &_lines,
    std::ostream &_target)
{
    const static std::string listChars = "-:;.,)]";
    std::string line;
    bool in_blockquote = false;
    int64_t ws_offset, prev_ws_offset = -1, list_depth = 0;

    for (auto it = _lines.begin(); it != _lines.end(); ++it)
    {
        line = *it;
        prev_ws_offset = ws_offset;
        ws_offset = 0;

        // Get new whitespace offset
        while (ws_offset < line.size() &&
               iswspace(line[ws_offset]))
        {
            ++ws_offset;
        }

        // Trim off whitespace for further processing
        line = line.substr(ws_offset);

        if (line.empty())
        {
            // Ignore empty lines
            continue;
        }
        else if (line[0] == '%')
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

            _target << line.substr(1) << '\n';

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
            _target << "}\\nopunct}~\n";

            _target << "\\bigskip{}\n";
            continue;
        }

        if (line[0] == '[')
        {
            // Link
            #error ""
            continue;
        }

        if (line[0] == '!')
        {
            // Image
            #error ""
            continue;
        }

        if (listChars.find(line[0]) != std::string::npos)
        {
            // Unnumbered list
            #error ""
            continue;
        }

        if (line.size() > 1 &&
                 (line.substr(0, 2) == "--" ||
                  line.substr(0, 2) == "~~" ||
                  line.substr(0, 2) == "__" ||
                  line.substr(0, 2) == "=="))
        {
            // Horizontal rule
            _target << "\\hrule{}\n";
            continue;
        }

        if (line.size() > 1 &&
                 (line[0] == '1' ||
                  line[0] == 'a' ||
                  line[0] == 'A') &&
                 listChars.find(line[1]) != std::string::npos)
        {
            // Numbered list
            #error ""
            continue;
        }

        // Normal text line
        _target << line << '\n';
        continue;
    }
}
