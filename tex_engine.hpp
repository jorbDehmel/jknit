/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
*/

#pragma once

#include "engine.hpp"
#include <set>
static_assert(__cplusplus >= 2020'00UL);

class TEXEngine : public Engine
{
  public:
    TEXEngine(const Settings &_s) : Engine(_s),
        forceFormalFont(_s.forceFancyFonts)
    {
    }

    const std::string specialCharacters = "%$~#&^";

    // If true, uses the default LaTeX font. If false, uses the
    // (IMO more visually appealling) sf font.
    bool forceFormalFont = false;

    const std::vector<std::string>
        latexHeader =
            {"\\documentclass[10pt]{article}",
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
             "\\usepackage{sectsty}",
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
             "\\begin{document}"},
        latexFooter = {"\\end{document}"},
        startCode = {"\\begin{code}"},
        endCode = {"\\end{code}\n"},
        startOutput = {"\\begin{codeoutput}"},
        endOutput = {"\\end{codeoutput}\n"},
        startMath = {"\\["}, endMath = {"\\]~\\\\"},
        startHeader = {"\\bf"}, endHeader;

    // did NOT have fun typing these
    std::set<std::string> lstSupportedLangs = {
        "ABAP",        "ACM",       "ACSL",        "ALGOL",
        "ASSEMBLER",   "BASH",      "C",           "C++",
        "CIL",         "COBOL",     "COMMAND.COM", "CSH",
        "EIFFEL",      "ELISP",     "EUPHORIA",    "GAP",
        "GNUPLOT",     "HANSL",     "HTML",        "INFORM",
        "JVMIS",       "LINGO",     "LLVM",        "LUA",
        "MATHEMATICA", "MERCURY",   "MIRANDA",     "ML",
        "MUPAD",       "OBERON-2",  "OCTAVE",      "OZ",
        "PERL",        "PL/I",      "POSTSCRIPT",  "PROLOG",
        "PSTRICKS",    "R",         "REXX",        "RUBY",
        "SAS",         "SCILAB",    "SHELXL",      "SPARQL",
        "SWIFT",       "TEX",       "VBSCRIPT",    "VHDL",
        "XML",         "ACMSCRIPT", "ADA",         "ANT",
        "AWK",         "BASIC",     "CAML",        "CLEAN",
        "COMSOL",      "DELPHI",    "ELAN",        "ERLANG",
        "FORTRAN",     "GCL",       "GO",          "HASKELL",
        "IDL",         "JAVA",      "KSH",         "LISP",
        "LOGO",        "MAKE",      "MATLAB",      "METAPOST",
        "MIZAR",       "MODULA-2",  "NASTRAN",     "OCL",
        "OOREXX",      "PASCAL",    "PHP",         "PLASM",
        "POV",         "PROMELA",   "PYTHON",      "REDUCE",
        "RSL",         "S",         "SCALA",       "SH",
        "SIMULA",      "SQL",       "TCL",         "VERILOG",
        "VRML",        "XSLT"};

  protected:
    void knit(const std::list<Chunk> &_chunks);

  private:
    void handle_md(const std::list<std::string> &_lines,
                   std::ostream &_target);
};
