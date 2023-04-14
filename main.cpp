/*
Jordan "Jorb" Dehmel
jdehmel@outlook.com
github.com/jorbDehmel
2023 - present
GPLv3 held by author
*/

#include "engine.hpp"
#include <chrono>
#include <iostream>
using namespace std;

#define VERSION "0.0.4"

// A better CLI for jknit
int main(const int argc, const char *argv[])
{
     string inputPath = "", outputPath = "jknit_output.tex";

     bool doLog = false;
     bool doTimer = false;
     bool quiet = false;

     for (int i = 1; i < argc; i++)
     {
          if (argv[i][0] == '-')
          {
               int cur = i;

               // Options
               for (int j = 1; argv[cur][j] != '\0'; j++)
               {
                    switch (argv[cur][j])
                    {
                    case 'O':
                    case 'o':
                         // Set output file, skip next option
                         if (cur + 1 >= argc)
                         {
                              cout << tags::red_bold
                                   << "Error: -o must be followed by a filepath.\n"
                                   << tags::reset;
                              return 2;
                         }
                         outputPath = argv[cur + 1];
                         i = cur + 1;
                         break;
                    case 'L':
                    case 'l':
                         // Do log
                         doLog = true;
                         break;
                    case 'T':
                    case 't':
                         // Do timer
                         doTimer = true;
                         break;
                    case 'Q':
                    case 'q':
                         // Quiet
                         quiet = true;
                         break;
                    case 'N':
                    case 'n':
                         // No compile
                         cout << tags::yellow_bold
                              << "-n called: Aborting.\n"
                              << tags::reset;
                         return 0;
                    case 'V':
                    case 'v':
                         // Version
                         cout << tags::green_bold
                              << VERSION << '\n'
                              << "GPLv3, jdehmel@outlook.com\n"
                              << "2023 - present\n"
                              << tags::reset;
                         break;
                    case 'H':
                    case 'h':
                         // Help
                         cout << tags::violet_bold
                              << "JKnit is a lightweight, versatile tool for\n"
                              << "compiling markdown documents with embedded\n"
                              << "(running) code to LaTeX. It aims to be easy\n"
                              << "to use with any embedded language. JKnit\n"
                              << "can compile most .rmd files with minimal\n"
                              << "additions.\n\n"
                              << "Flags and their meaning:\n"
                              << " -o \t Set output file to the next argument\n"
                              << " -l \t Enable log\n"
                              << " -t \t Enable timer\n"
                              << " -q \t Quiet (no printing)\n"
                              << " -n \t No compile (halt before running)\n"
                              << " -v \t Show version\n"
                              << " -h \t Show help (this)\n\n"
                              << "Jorb Dehmel, 2023, jdehmel@outlook.com\n"
                              << "FOSS, Protected by GPLv3\n"
                              << VERSION << '\n'
                              << tags::reset;

                         break;
                    default:
                         cout << tags::yellow_bold
                              << "Unrecognized option '" << argv[cur][j] << "'\n"
                              << "Note: Use -h for a list of all commands.\n"
                              << tags::reset;
                         break;
                    };
               }
          }
          else
          {
               if (inputPath == "")
               {
                    inputPath = argv[i];
               }
               else
               {
                    cout << tags::yellow_bold
                         << "Warning: Interpreting second filename arg as output file\n"
                         << "(Please use -o notation in the future)\n"
                         << tags::reset;

                    outputPath = argv[i];
               }
          }
     }

     if (inputPath == "")
     {
          cout << tags::red_bold
               << "Please specify an input path.\n"
               << tags::reset;
          return 1;
     }

     Engine e(doLog);

     // Interpretted languages
     e.fromString("python python3 print('CHUNK_BREAK') py\n");
     e.fromString("octave octave-cli disp('CHUNK_BREAK') txt\n");
     e.fromString("bash /usr/bin/sh 'echo CHUNK_BREAK' sh");
     e.fromString("js node 'console.log('CHUNK_BREAK');' js");

     // Compiled loner languages
     e.fromString("clang++* /usr/include/compilation-drivers/clang-driver.py ; cpp");
     e.fromString("gcc* /usr/include/compilation-drivers/gcc-driver.py ; cpp");
     e.fromString("rust* /usr/include/compilation-drivers/rustc-driver.py ; rs");

     if (!quiet)
     {
          cout << tags::green_bold
               << "Compiling from '" << inputPath << "' to '" << outputPath << "'...\n"
               << tags::reset << flush;
     }

     try
     {
          if (doTimer && !quiet)
          {
               // Timed
               auto start = chrono::high_resolution_clock::now();

               e.processFile(inputPath, outputPath);

               auto end = chrono::high_resolution_clock::now();
               unsigned long long int elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
               cout << tags::violet_bold
                    << "Elapsed ms: " << elapsed << '\n'
                    << "Ms waiting for code chunk output: " << systemWaitMS << '\n'
                    << "JKnit-attributable ms: " << elapsed - systemWaitMS << '\n'
                    << "Percent code-chunk-attributable: " << 100 * (double)systemWaitMS / elapsed << "%\n"
                    << tags::reset;
          }
          else
          {
               // Untimed
               e.processFile(inputPath, outputPath);
          }
     }
     catch (...)
     {
          e.log.close();
          cout << tags::red_bold
               << "A fatal error ocurred.\n"
               << tags::reset;
          return 4;
     }

     if (!quiet)
     {
          cout << tags::green_bold
               << "Done.\n"
               << tags::reset;
     }

     return 0;
}
