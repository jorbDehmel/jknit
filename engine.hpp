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
        "\\usepackage{moresize}"
        "\\geometry{letterpaper}",
        "\\usepackage{xcolor}",
        "\\usepackage{color}",
        "\\usepackage{amsmath}",
        "\\usepackage{amssymb}",
        "\\usepackage[many]{tcolorbox}",
        "\\tcbuselibrary{listings}",
        "\\newtcblisting{code} {",
        "    listing only,",
        "    breakable,",
        "    colback = lightgray,",
        "    boxrule = 0pt,",
        "    listing options = {",
        "        basicstyle =\\ttfamily,",
        "        breaklines = true,",
        "        columns = fullflexible}}",
        "\\newtcblisting{codeoutput} {",
        "    listing only,",
        "    breakable,",
        "    colback = white,",
        "    boxrule = 1pt,",
        "    colframe = gray,",
        "    listing options = {"
        "        basicstyle =\\ttfamily,",
        "        breaklines = true,",
        "        columns = fullflexible}}",
        "\\newenvironment{pres}{\\begin{landscape}\\Huge}{\\end{landscape}}",
        "\\begin{document}",
        "\\sffamily"};

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
