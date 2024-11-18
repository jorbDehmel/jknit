/*
JKnit base engine. This inserts code output into Markdown
documents, leaving the specific output to a child class.
2023 - present
Jordan Dehmel
*/

#pragma once

#include <chrono>
#include <cstdint>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <string>

const static std::string VERSION = "0.1.4";

struct Settings
{
    std::string source, target;
    bool time = false, log = false, all_errors = false,
         forceFancyFonts = false;
};

struct RunStats
{
    std::chrono::high_resolution_clock::time_point start, stop;
    uint64_t external_us = 0;
};

struct Builder
{
    std::string printChunkBreak, commandPath, extension;
};

// A text or code chunk. There are four types here: Text
// (markdown), code, resolved code output, and unresolved code
// output. Unresolved code output chunks contain some
// identifying information such that their true output can be
// recovered in the second parsing pass.
struct Chunk
{
    std::string type;
    uint64_t pos_in_type;
    std::list<std::string> lines;

    bool show_code = true, show_output = true, combine = true;
};

// Virtual base class; This does not say how to implement
// `knit`, although the rest of the methods are implemented. A
// child class may target markdown or tex, in which case we want
// to leave the specific output virtual. To inherit, implement
// the `knit` method.
class Engine
{
  public:
    Engine(const Settings &_s);
    ~Engine();

    void load_settings_file(const std::string &_filepath);
    void load_settings_line(const std::string &_line);

    RunStats run();

  protected:
    Settings settings;
    std::ifstream source;
    std::ofstream target, log;
    std::map<std::string, Builder> builders;

    // Pseudo-RNG to help avoid local collisions in filenames
    const std::string magic_number = std::to_string(time(NULL));

    // Run a code chunk and return its output as a chunk
    Chunk run_code_chunk(const Builder &_builder,
                         const Chunk &_code);

    // Run the given shell command and get its output.
    uint64_t external_us = 0;
    Chunk run_and_get_output(const std::string &_cmd);

    // Break a single output chunk into multiple
    std::queue<Chunk> break_output_chunk(const Chunk &_c);

    // Go through the input, extract code from `jmd` to output.
    // This creates a list of text/code chunks which should then
    // be constructed into output.
    std::list<Chunk> parse();

    // Constructs a series of text/code chunks into the output
    // file. This is abstract, as the specific language
    // targetted may vary.
    virtual void knit(const std::list<Chunk> &_chunks) = 0;
};
