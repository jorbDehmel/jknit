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
    ifstream settings("jknit.jk");
    assert(settings.is_open());
    Engine e(settings);
    settings.close();

    cout << "Loaded.\n";

    e.processFile("test.pyd", "test.tex");

    return 0;
}
