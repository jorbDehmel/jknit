/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include <iostream>
#include <chrono>
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

    cout << tags::green_bold
         << "Compiling from " << argv[1] << " to " << argv[2] << '\n'
         << tags::reset;

    auto start = chrono::high_resolution_clock::now();

    Engine e(true);
    e.processFile(argv[1], argv[2]);

    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << tags::green_bold
         << "Done.\n"
         << elapsed << " ns\n"
         << elapsed / 1'000'000. << " ms\n"
         << elapsed / 1'000'000'000. << " s\n"
         << tags::reset;

    smartSys(string("pdflatex ") + argv[2]);

    return 0;
}
