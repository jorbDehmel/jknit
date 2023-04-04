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
    if (argc < 3)
    {
        cout << tags::red_bold
             << "Error: Please provide two arguments (input and output filenames).\n"
             << tags::reset;
        return 1;
    }

    cout << "Compiling from " << argv[1] << " to " << argv[2] << '\n';

    Engine e;
    e.processFile(argv[1], argv[2]);
    smartSys(string("pdflatex ") + argv[2]);

    return 0;
}
