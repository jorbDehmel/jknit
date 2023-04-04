#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <set>
#include <fstream>
#include <cassert>
#include <vector>
#include "tags.hpp"
using namespace std;

struct builder
{
    string commandPath;
};

extern string buildSpace;

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
        "\\usepackage[margin=0.5in]{geometry}",
        "\\usepackage{csquotes}",
        "\\usepackage{graphicx}",
        "\\usepackage{hyperref}",
        "\\geometry{letterpaper}",
        "\\begin{document}"};

    string latexFooter = "\\end{document}";

    string startCode = "\\begin{verbatim}\n", endCode = "\n\\end{verbatim}";
    string startOutput = "\\begin{verbatim}", endOutput = "\n\\end{verbatim}";
    string startMath = "\\[", endMath = "\\]";

    bool doLog = false;
    ofstream log;

protected:
    map<string, builder> builders;
};

#endif
