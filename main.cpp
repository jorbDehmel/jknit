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
#include <filesystem>
using namespace std;

#define VERSION "0.0.6"

// A better CLI for jknit
int main(const int argc, const char *argv[])
{
     string inputPath = "", outputPath = "jknit_output.tex";

     bool doLog = false;
     bool doTimer = false;
     bool quiet = false;

     debug = false;
     failWithCode = false;

     vector<string> fromFiles;

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
                         if (i + 1 >= argc)
                         {
                              cout << tags::red_bold
                                   << "Error: -o must be followed by a filepath.\n"
                                   << tags::reset;
                              return 2;
                         }
                         i++;
                         outputPath = argv[i];
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
                    case 'F':
                    case 'f':
                         // From next file
                         if (i + 1 >= argc)
                         {
                              cout << tags::red_bold
                                   << "Error: -f must be followed by a filepath.\n"
                                   << tags::reset;
                              return 2;
                         }
                         i++;
                         fromFiles.push_back(argv[i]);
                         break;
                    case 'D':
                    case 'd':
                         debug = true;
                         break;
                    case 'E':
                    case 'e':
                         failWithCode = true;
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
                              << " -f \t From file (load settings)\n"
                              << " -d \t Debug mode\n"
                              << " -e \t Fail on code error\n"
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

#if (defined(_WIN32) || defined(_WIN64))
          // In linux, failure is ok; You're already there
          cin >> inputPath;
#else
          return 1;
#endif
     }

     Engine e(doLog);

#if (defined(_WIN32) || defined(_WIN64))
     // No guarentees that the tags will work
     cout << tags::red_bold
          << "Warning: Windows is not fully supported! You may have to manually enter paths.\n"
          << tags::reset;

     cout << "Converted filepath from " << inputPath << " to ";
     for (unsigned int i = 0; i < inputPath.size(); i++)
     {
          if (inputPath[i] == '/')
          {
               inputPath[i] = '\\';
          }
     }
     cout << inputPath << '\n';

     cout << "Converted filepath from " << outputPath << " to ";
     for (unsigned int i = 0; i < outputPath.size(); i++)
     {
          if (outputPath[i] == '/')
          {
               outputPath[i] = '\\';
          }
     }
     cout << outputPath << '\n';

     // Detect and set octave path
     try
     {
          string path;
          try
          {
               string path = "C:\\Program Files\\GNU Octave";
               for (const auto &p : filesystem::directory_iterator(path))
               {
                    path = p.path().string();
                    break;
               }
          }
          catch (...)
          {
               string path = "C:\\Octave";
               for (const auto &p : filesystem::directory_iterator(path))
               {
                    path = p.path().string();
                    break;
               }
          }

          path += "\\mingw64\\bin\\octave-cli.exe";
          cout << path << '\n';

          e.fromString("octave '" + path + "' disp('CHUNK_BREAK'); txt");
     }
     catch (...)
     {
          cout << "Could not locate Octave. Please ensure it is installed.\n";
     }

     // Detect and set python path
     try
     {
          string path = "%%USERPATH%%\\AppData\\Local\\Programs\\Python\\";
          for (const auto &p : filesystem::directory_iterator(path))
          {
               path = p.path().string();
               break;
          }

          path += "\\python.exe";
          cout << path << '\n';

          e.fromString("python '" + path + "' print('CHUNK_BREAK'); py");
     }
     catch (...)
     {
          cout << "Could not locate Python. Please ensure it is installed.\n";
     }
#else
     // I hate windows so much, this is all so easy on linux

     // Interpretted languages
     e.fromString("python python3 print('CHUNK_BREAK') py\n");
     e.fromString("octave octave-cli disp('CHUNK_BREAK') txt\n");
     e.fromString("bash /usr/bin/sh 'echo CHUNK_BREAK' sh");
     e.fromString("js node 'console.log('CHUNK_BREAK');' js");

     // Compiled loner languages
     e.fromString("clang++* /usr/include/compilation-drivers/clang-driver.py ; cpp");
     e.fromString("gcc* /usr/include/compilation-drivers/gcc-driver.py ; cpp");
     e.fromString("rust* /usr/include/compilation-drivers/rustc-driver.py ; rs");
#endif

     for (auto from : fromFiles)
     {
          // Load to string
          ifstream file(from);
          if (!file.is_open())
          {
               cout << tags::red_bold
                    << "Error: Could not open settings file '" << from << "'. Skipping.\n"
                    << tags::reset;
               continue;
          }

          string contents, line;
          while (getline(file, line))
          {
               contents += line + '\n';
          }
          file.close();

          // Load engine from this string
          e.fromString(contents);
     }

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
     catch (const runtime_error &error)
     {
          e.log.close();
          cout << tags::red_bold
               << "Error: " << error.what()
               << tags::reset;
          return 4;
     }
     catch (...)
     {
          e.log.close();
          cout << tags::red_bold
               << "An unknown fatal error ocurred.\n"
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
