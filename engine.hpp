#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <chrono>
#include "tags.hpp"
using namespace std;

// For system independence
// Luckily, output redirecting via > works as-is
// on DOS cuz that's basically all this program is
#if (defined(_WIN32) || defined(_WIN64))
// Windows because it just has to be different
const string rm = "rmdir /s /q ";
const string mkdir = "md ";
#else
// UNIX-based OSes
const string rm = "rm -rf ";
const string mkdir = "mkdir -p ";
#endif

struct builder
{
    string commandPath;
};

extern string buildSpace;

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

    vector<string> latexHeader = {
        "\\documentclass[12pt]{amsart}",
        "\\usepackage[margin=1in]{geometry}",
        "\\usepackage{csquotes}",
        "\\usepackage{graphicx}",
        "\\usepackage{hyperref}",
        "\\geometry{letterpaper}",
        "\\usepackage{xcolor}",
        "\\usepackage{color}",
        "\\usepackage{tcolorbox}",
        "\\sffamily",
        "\\begin{document}"};

    vector<string> latexFooter = {
        "\\end{document}"};

    vector<string> startCode = {
        "\\begin{tcolorbox}[colback=lightgray, boxrule=0pt]",
        "\\begin{verbatim}\n"};

    vector<string> endCode = {
        "\n\\end{verbatim}",
        "\\end{tcolorbox}\n"};

    vector<string> startOutput = {
        "\\begin{verbatim}"};

    vector<string> endOutput = {
        "\n\\end{verbatim}"};

    vector<string> startMath = {
        "\\["};

    vector<string> endMath = {
        "\\]"};

    vector<string> startHeader = {
        "\\sffamily \\bf"};

    bool doLog = false;
    ofstream log;

protected:
    map<string, builder> builders;
};

#endif
