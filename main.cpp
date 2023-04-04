/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <iostream>
#include "engine.hpp"
using namespace std;

int main(const int argc, const char *argv[])
{
    Engine e;

    e.processFile("test.jmd", "test.tex");

    smartSys("pdflatex test.tex");

    return 0;
}
