#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <set>
#include <fstream>
#include <cassert>
#include "tags.hpp"
using namespace std;

struct builder
{
    string commandPath;
    string options;
    string ext;
};

extern string buildSpace;

void smartSys(const string &Command);

class Engine
{
public:
    Engine(){};
    Engine(const string &From);
    Engine(istream &From);

    void processFile(const string &InputFilepath, const string &OutputFilepath);

    void processChunk(const string Header, const string &Contents, ostream &Stream);

    void setPath(const string &Name, const string &Path);

    void setOptions(const string &Name, const string &Options);
    void appendOptions(const string &Name, const string &Options);

    bool hasPath(const string &What) const;

    string toString() const;
    void fromString(const string &From);

    void toStream(ostream &Stream) const;

    string latexHeader = "\\documentclass[12pt]{amsart}\n\\usepackage[margin=0.5in]{geometry}\n\\usepackage{graphicx}\n\\geometry{letterpaper}\n\\begin{document}";
    string latexFooter = "\\end{document}";

    string startCode = "\\begin{verbatim}\n", endCode = "\n\\end{verbatim}";
    string startOutput = "\\begin{verbatim}", endOutput = "\n\\end{verbatim}";
    string startMath = "\\[", endMath = "\\]";

protected:
    map<string, builder> builders;
};

#endif
