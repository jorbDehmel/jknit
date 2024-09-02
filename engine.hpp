/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

const static std::string VERSION = "0.0.12";

// For system independence
// Luckily, output redirecting via > works as-is
// on DOS cuz that's basically all this program is
#if (defined(_WIN32) || defined(_WIN64))
// Windows because it just has to be different
const std::string buildSpace = "jknit\\";
#else
// UNIX-based OSes
const std::string buildSpace = "jknit/";
#endif

struct builder
{
    std::string commandPath;
    std::string printChunkBreak;
    std::string extension = "txt";
};

class Engine
{
  public:
    Engine(bool _debug = false, bool _failWithCode = false,
           bool _doLog = false);
    ~Engine();

    // Used to be globals
    bool debug, failWithCode, doLog;
    unsigned long long int systemWaitMS;

    void smartSys(const std::string &Command,
                  std::ostream &Stream = std::cout);

    void processFile(const std::string &InputFilepath,
                     const std::string &OutputFilepath,
                     const bool &PresMode);

    void processChunk(const std::string Header,
                      const std::string &Contents,
                      std::ostream &Stream);

    void setPath(const std::string &Name,
                 const std::string &Path);

    void setOptions(const std::string &Name,
                    const std::string &Options);
    void appendOptions(const std::string &Name,
                       const std::string &Options);

    bool hasPath(const std::string &What) const;

    std::string toString() const;
    void fromString(const std::string &From);

    void toStream(std::ostream &Stream) const;

    // Turns a line from markdown into latex and inserts it into
    // a stream
    void processMDLine(const std::string &Line,
                       std::ostream &Stream);

    const std::string specialCharacters = "%$~#&^";

    std::vector<std::string>
        latexHeader =
            {"\\documentclass[10pt]{amsart}",
             "\\usepackage[margin=1in]{geometry}",
             "\\usepackage{background}",
             "\\usepackage{csquotes}",
             "\\usepackage{graphicx}",
             "\\usepackage{hyperref}",
             "\\usepackage{pdflscape}",
             "\\usepackage{relsize}",
             "\\usepackage{moresize}",
             "\\usepackage[dvipsnames]{xcolor}",
             "\\usepackage{color}",
             "\\usepackage{amsmath}",
             "\\usepackage{amssymb}",
             "\\usepackage[many]{tcolorbox}",
             "\\usepackage{afterpage}",
             "\\tcbuselibrary{listings}",
             "\\geometry{letterpaper}",
             "\\newtcblisting{code} {",
             "listing only,",
             "breakable,",
             "boxrule = 1pt,",
             "colframe = gray,",
             "listing options = {",
             "basicstyle = \\ttfamily\\relsize{-1},",
             "breaklines = true,",
             "columns = fullflexible,",
             "commentstyle = \\color{olive},",
             "keywordstyle = \\color{MidnightBlue},",
             "stringstyle = \\color{OliveGreen},",
             "breakatwhitespace = false,",
             "keepspaces = true,",
             "numbersep = 5pt,",
             "showspaces = false,",
             "showstringspaces = false,",
             "showtabs = false,",
             "tabsize = 2}} ",
             "\\newtcblisting{codeoutput}{",
             "listing only,",
             "breakable,",
             "colback = white,",
             "boxrule = 1pt,",
             "colframe = gray,",
             "listing options = {",
             "basicstyle =\\ttfamily\\relsize{-1},",
             "breaklines = true,",
             "columns = fullflexible}}",
             "\\newenvironment{pres}{\\begin{landscape}"
             "\\Huge\\pagestyle{empty}\\clearpage}{\\end{"
             "landscape}"
             "}",
             "\\newenvironment{titleslide}{"
             "\\HUGE\\vspace*{0.1\\textheight}\\afterpage{"
             "\\nopagecolor}}{}",
             "\\newcommand{\\slide}{\\newpage{}\\nopagecolor}",
             "\\newcommand{\\bgimg}[1]{\\backgroundsetup{angle="
             "90,"
             "scale=1,contents={"
             "\\includegraphics[width=\\paperwidth,height="
             "\\paperheight]{#1}}}}",
             "\\begin{document}",
             "\\sffamily"},
        latexFooter = {"\\end{document}"},
        startCode = {"\\begin{code}"},
        endCode = {"\\end{code}\n"},
        startOutput = {"\\begin{codeoutput}"},
        endOutput = {"\\end{codeoutput}\n"},
        startMath = {"\\["}, endMath = {"\\]~\\\\"},
        startHeader = {"\\bf"}, endHeader;

    std::ofstream log;

    // did NOT have fun typing these
    std::set<std::string> lstSupportedLangs = {
        "ABAP",        "ACM",       "ACSL",        "Algol",
        "Assembler",   "bash",      "C",           "C++",
        "CIL",         "Cobol",     "command.com", "csh",
        "Eiffel",      "elisp",     "Euphoria",    "GAP",
        "Gnuplot",     "hansl",     "HTML",        "inform",
        "JVMIS",       "Lingo",     "LLVM",        "Lua",
        "Mathematica", "Mercury",   "Miranda",     "ML",
        "MuPAD",       "Oberon-2",  "Octave",      "Oz",
        "Perl",        "PL/I",      "PostScript",  "Prolog",
        "PSTricks",    "R",         "Rexx",        "Ruby",
        "SAS",         "Scilab",    "SHELXL",      "SPARQL",
        "Swift",       "TeX",       "VBScript",    "VHDL",
        "XML",         "ACMscript", "Ada",         "Ant",
        "Awk",         "Basic",     "Caml",        "Clean",
        "Comsol",      "Delphi",    "Elan",        "erlang",
        "Fortran",     "GCL",       "Go",          "Haskell",
        "IDL",         "Java",      "ksh",         "Lisp",
        "Logo",        "make",      "Matlab",      "MetaPost",
        "Mizar",       "Modula-2",  "NASTRAN",     "OCL",
        "OORexx",      "Pascal",    "PHP",         "Plasm",
        "POV",         "Promela",   "Python",      "Reduce",
        "RSL",         "S",         "Scala",       "sh",
        "Simula",      "SQL",       "tcl",         "Verilog",
        "VRML",        "XSLT"};

  protected:
    std::map<std::string, builder> builders;

    // Scans for code chunks, collates them by builder, and
    // compiles. Splits by inserting a builder's printChunkBreak
    // AFTER each chunk runs
    void buildAllChunks(const std::string &FileContents);

    // Maps a language to its code chunk outputs in order
    std::map<std::string, std::vector<std::string>>
        chunkOutputs;
    std::map<std::string, int> curChunkByLang;
};

#endif
