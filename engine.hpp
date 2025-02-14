/**
 * @brief JKnit base engine. This inserts code output into
 * Markdown documents, leaving the specific output to a child
 * class.
 * @year 2023 - present
 * @author Jordan Dehmel
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <string>

/// A std::string representing the jknit core version (x.y.z)
const static std::string VERSION = "0.1.4";

/**
 * @struct Settings
 * @brief The object passed to an engine to specify the desired
 * output.
 */
struct Settings
{
    /// The source file (md or jmd)
    std::string source;

    /// The target file (md or tex)
    std::string target;

    /// Whether or not to time the run
    bool time = false;

    /// Whether or not to log the run
    bool log = false;

    /// Whether or not to display all errors
    bool all_errors = false;

    /// If true, uses the "fancy" default tex font instead of
    /// the friendlier rmd font
    bool forceFancyFonts = false;
};

/**
 * @struct RunStats
 * @brief The object returned after a run describing how it went
 */
struct RunStats
{
    /// The start and stop time points of the run
    std::chrono::high_resolution_clock::time_point start, stop;

    /// The number of microseconds spent externally
    uint64_t external_us = 0;
};

/**
 * @struct Builder
 * @brief Specifies how a single chunk language behaves
 */
struct Builder
{
    /// How to print the chunk break (only possible sometimes)
    std::string printChunkBreak;

    /// The "run" command (not just compile, run)
    std::string commandPath;

    /// The needed file extension (only needed sometimes)
    std::string extension;
};

/**
 * @struct Chunk
 * @brief A text or code chunk. There are four types here: Text
 * (markdown), code, resolved code output, and unresolved code
 * output. Unresolved code output chunks contain some
 * identifying information such that their true output can be
 * recovered in the second parsing pass.
 */
struct Chunk
{
    /// The type of this chunk (text, code, resolved code, or
    /// code output)
    std::string type;

    ///
    uint64_t pos_in_type;

    /// The contents of the chunk
    std::list<std::string> lines;

    /// Whether to display code
    bool show_code = true;

    /// Whether to display output
    bool show_output = true;

    /// Whether it shares the same file as others when run
    bool combine = true;
};

/**
 * @class Engine
 * @brief Virtual base class; This does not say how to implement
 * `knit`, although the rest of the methods are implemented. A
 * child class may target markdown or tex, in which case we want
 * to leave the specific output virtual. To inherit, implement
 * the `knit` method.
 */
class Engine
{
  public:
    /// Instantiate from the given settings
    Engine(const Settings &_s);

    /// Load a settings file (a series of lines)
    void load_settings_file(const std::string &_filepath);

    /// Load a single line of settings
    void load_settings_line(const std::string &_line);

    /// Run and yield statistics
    RunStats run();

  protected:
    /// Run settings
    Settings settings;

    /// The source file (opened in constructor)
    std::ifstream source;

    /// The target and optionally log files (opened in
    /// constructor)
    std::ofstream target, log;

    /// All the known builders. Aliases are legal
    std::map<std::string, Builder> builders;

    /// RNG to help avoid local collisions in filenames
    const std::string magic_number =
        std::to_string(std::random_device()());

    /// Run a code chunk and return its output as a chunk
    Chunk run_code_chunk(const Builder &_builder,
                         const Chunk &_code);

    /// Logs the cumulative us of external commands
    uint64_t external_us = 0;

    /// Run the given shell command and get its output.
    Chunk run_and_get_output(const std::string &_cmd);

    /// Break a single output chunk into multiple
    std::queue<Chunk> break_output_chunk(const Chunk &_c);

    /// Go through the input, extract code from `jmd` to output.
    /// This creates a list of text/code chunks which should
    /// then be constructed into output.
    std::list<Chunk> parse();

    /// Constructs a series of text/code chunks into the output
    /// file. This is abstract, as the specific language
    /// targetted may vary.
    virtual void knit(const std::list<Chunk> &_chunks) = 0;
};
