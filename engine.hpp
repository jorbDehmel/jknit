/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <set>

#include "tags.hpp"
using namespace std;

// For system independence
// Luckily, output redirecting via > works as-is
// on DOS cuz that's basically all this program is
#if (defined(_WIN32) || defined(_WIN64))
// Windows because it just has to be different
const string rm = "rmdir /s /q ";
const string mkdir = "md ";
const string buildSpace = "jknit\\";
#else
// UNIX-based OSes
const string rm = "rm -rf ";
const string mkdir = "mkdir -p ";
const string buildSpace = "jknit/";
#endif

struct builder
{
    string commandPath;
    string printChunkBreak;
    string extension = "txt";
};

extern bool debug, failWithCode;

extern unsigned long long int systemWaitMS;
void smartSys(const string &Command, ostream &Stream = cout);

class Engine
{
public:
    Engine(const bool &DoLog = false);
    ~Engine();

    void processFile(const string &InputFilepath, const string &OutputFilepath);

    void processChunk(const string Header, const string &Contents, ostream &Stream);

    void setPath(const string &Name, const string &Path);

    void setOptions(const string &Name, const string &Options);
    void appendOptions(const string &Name, const string &Options);

    bool hasPath(const string &What) const;

    string toString() const;
    void fromString(const string &From);

    void toStream(ostream &Stream) const;

    const string specialCharacters = "%$_~#&^";

    vector<string> latexHeader = {
        "\\documentclass[10pt]{amsart}",
        "\\usepackage[margin=1in]{geometry}",
        "\\usepackage{csquotes}",
        "\\usepackage{graphicx}",
        "\\usepackage{hyperref}",
        "\\usepackage{pdflscape}",
        "\\usepackage{relsize}",
        "\\usepackage{moresize}"
        "\\geometry{letterpaper}",
        "\\usepackage[usenames,dvipsnames]{xcolor}",
        "\\usepackage{color}",
        "\\usepackage{amsmath}",
        "\\usepackage{amssymb}",
        "\\usepackage[many]{tcolorbox}",
        "\\tcbuselibrary{listings}",
        "\\newtcblisting{code} {",
        "   listing only,",
        "   breakable,",
        "   boxrule = 1pt,",
        "   colframe = gray,",
        "   listing options = {",
        "       basicstyle = \\ttfamily\\relsize{-1},",
        "       breaklines = true,",
        "       columns = fullflexible,",
        "       commentstyle = \\color{olive},",
        "       keywordstyle = \\color{MidnightBlue},",
        "       stringstyle = \\color{OliveGreen},",
        "       breakatwhitespace = false,",
        "       keepspaces = true,",
        "       numbersep = 5pt,",
        "       showspaces = false,",
        "       showstringspaces = false,",
        "       showtabs = false,",
        "       tabsize = 2}} ",
        "\\newtcblisting{codeoutput}{",
        "    listing only,",
        "    breakable,",
        "    colback = white,",
        "    boxrule = 1pt,",
        "    colframe = gray,",
        "    listing options = {",
        "        basicstyle =\\ttfamily\\relsize{-1},",
        "        breaklines = true,",
        "        columns = fullflexible}}",
        "\\newenvironment{pres}{\\begin{landscape}\\Huge}{\\end{landscape}}",
        "\\newcommand{\\slide}{\\newpage{}}", "\\begin{document}", "\\sffamily"};

    vector<string> latexFooter = {
        "\\end{document}"};

    vector<string> startCode = {
        "\\begin{code}"};

    vector<string> endCode = {
        "\\end{code}\n"};

    vector<string> startOutput = {
        "\\begin{codeoutput}"};

    vector<string> endOutput = {
        "\\end{codeoutput}\n"};

    vector<string> startMath = {
        "\\["};

    vector<string> endMath = {
        "\\]~\\\\"};

    vector<string> startHeader = {
        "\\bf"};

    vector<string> endHeader;

    bool doLog = false;
    ofstream log;

    // did NOT have fun typing these
    set<string> lstSupportedLangs = {
        "ABAP",
        "ACM",
        "ACSL",
        "Algol",
        "Assembler",
        "bash",
        "C",
        "C++",
        "CIL",
        "Cobol",
        "command.com",
        "csh",
        "Eiffel",
        "elisp",
        "Euphoria",
        "GAP",
        "Gnuplot",
        "hansl",
        "HTML",
        "inform",
        "JVMIS",
        "Lingo",
        "LLVM",
        "Lua",
        "Mathematica",
        "Mercury",
        "Miranda",
        "ML",
        "MuPAD",
        "Oberon-2",
        "Octave",
        "Oz",
        "Perl",
        "PL/I",
        "PostScript",
        "Prolog",
        "PSTricks",
        "R",
        "Rexx",
        "Ruby",
        "SAS",
        "Scilab",
        "SHELXL",
        "SPARQL",
        "Swift",
        "TeX",
        "VBScript",
        "VHDL",
        "XML",
        "ACMscript",
        "Ada",
        "Ant",
        "Awk",
        "Basic",
        "Caml",
        "Clean",
        "Comsol",
        "Delphi",
        "Elan",
        "erlang",
        "Fortran",
        "GCL",
        "Go",
        "Haskell",
        "IDL",
        "Java",
        "ksh",
        "Lisp",
        "Logo",
        "make",
        "Matlab",
        "MetaPost",
        "Mizar",
        "Modula-2",
        "NASTRAN",
        "OCL",
        "OORexx",
        "Pascal",
        "PHP",
        "Plasm",
        "POV",
        "Promela",
        "Python",
        "Reduce",
        "RSL",
        "S",
        "Scala",
        "sh",
        "Simula",
        "SQL",
        "tcl",
        "Verilog",
        "VRML",
        "XSLT"};

protected:
    map<string, builder> builders;

    // Scans for code chunks, collates them by builder, and compiles.
    // Splits by inserting a builder's printChunkBreak AFTER each chunk runs
    void buildAllChunks(const string &FileContents);

    // Maps a language to its code chunk outputs in order
    map<string, vector<string>> chunkOutputs;
    map<string, int> curChunkByLang;
};

#endif
