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

    string processChunk(const string Header, const string &Contents);

    void setPath(const string &Name, const string &Path);

    void setOptions(const string &Name, const string &Options);
    void appendOptions(const string &Name, const string &Options);

    bool hasPath(const string &What) const;

    string toString() const;
    void fromString(const string &From);

    void toStream(ostream &Stream) const;

    string latexHeader = "\\documentclass[12pt]{amsart}\n\\usepackage[margin=0.5in]{geometry}\n\\geometry{letterpaper}\n\\begin{document}\n";
    string latexFooter = "\\end{document}\n";

protected:
    map<string, builder> builders;
};

#endif
