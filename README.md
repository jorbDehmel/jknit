# JKnit

Jordan Dehmel \
2023 - Present \
jdehmel@outlook.com \
`github.com/jorbDehmel/jknit`

JKnit is an extremely lightweight mathematical/computational
document knitting tool. It is purely FOSS forever, and aims to
intuitive, easy and fast. It compiles source files composed of
code embedded in documentation into pure documentation files.
This allows running code within good looking, feature-rich
documents.

JKnit takes in `jmd` (similar to `rmd`) files and targets either
`md` or `tex`.

JKnit allows internal running code from the following languages
by default.

Interpreted languages:
- Python 3 via `python3`
- `Octave`
- `R`
- `bash`
- JavaScript via `node`

Compiled (non-knit) languages:
- C++ via `clang++`
- C++ via `g++`
- C via `clang`
- C via `gcc`
- Rust via `rustc`
- [Oak](https://github.com/jorbDehmel/oak) via `acorn`

## Installation requirements

These are actually just suggestions, but you will find that
JKnit will have little function without them.

 1 - Python
 2 - Octave (MatLab alternative)
 3 - Cargo (for Rust)
 4 - Nodejs (for JavaScript)
 5 - Clang (for C++, via LLVM)
 6 - `gcc`, `g++`
 7 - MikTex, TeXLive, `pandoc`

All but the last of these items allow JKnit to compile and/or
run their respective languages. The final one, however, is used
to translate JKnit's output `.tex` files into more useful
formats like `.pdf`.

## Linux Installation

Simply run the command `make install` while in this directory.
Similarly, run `make uninstall` to uninstall it. JKnit is
purposefully lightweight, and takes only a few megabytes of
storage for the binary and compilation drivers.

## The CLI (Command Line Interface)

Command line flags and their meanings are shown below.

 Flag | Meaning
------|---------------------------------------------------------
 `t`  | Toggle timer (default off)
 `l`  | Toggle log (default off)
 `o`  | Set target (default `a.md`)
 `f`  | Load settings file
 `v`  | Print version
 `e`  | Toggle warnings-to-errors mode (default off)
 `x`  | Force the output language to be `tex`
 `h`  | Print help

For example, `jknit foo.jmd -hxeo foo.txt` would print help
text (`h`), force the output to be `tex` (`x`), turn all
warnings to errors (`e`), and set the target file to `foo.txt`
(`o` followed by `foo.txt`).

## Running Code

By default, JKnit inserts the output of code after its source in
a markdown document. For instance, the markdown

\```python \
print('Hello, world!') \
\```

Becomes

\```python \
print('Hello, world!') \
\```

\```txt \
Hello, world! \
\```

The language is specified by the string following the three
backticks (for `md` support), and can optionally be enclosed by
curly brackets (for `rmd` support).

## Loading Settings

To include support for an additional interpreted language,
simply include the following code chunk at the top of your
document.

\`\`\`settings \
name command printChunkBreak extension \
\`\`\`

"name" is what you will put inside the curly brackets of future
code chunks in order to call the given command.

"printChunkBreak" is a command which will print the string
"CHUNK_BREAK" to the command line. This is **very important!**
This is what will be used to tell the chunks apart.

The "extension" is the file extension associated with that
language.

For instance, the following code will add support for the cling
`C++` interpreter.

\`\`\`settings \
cling cling cout<<"CHUNK_BREAK\n"; cpp \
\`\`\`

If you wish to include spaces in the command or printChunkBreak
sections, surround them in either single or double quotation
marks.

## Chunk Options

 Operator | Purpose
----------|-------------
 `*`      | Lone chunk
 `^`      | Hide output
 `~`      | Hide code

Operator combos and examples:
`*^` - `C++` code without output
`*~` - `C++` output without code
`^~` - Load-bearing but ugly `Python` code
`*^~` - Code which has no effect and is not shown: Ignored.

## Adding Language Support

### Compiled Languages

Compiled languages must have a supplemental script written for
them. This script must take a filepath as an input and echo any
printed text.

For instance, to implement truly compiled C++ code, you would
need to write a supplemental script that does the following.

 1. Take the .cpp file as a command line argument
 2. Compile the file
 3. Run the executable and echo its output

Several such "compilation drivers" are included upon install.

Since a useful compiled chunk will include a main function and
only one main function can be compiled, it is mostly useful to
use these with the '\*' lone operator, which makes a chunk run
disconnected from any other. This will cause JKnit not to insert
any instances of the chunk-break print line, which will usually
cause compilation failures in such languages.

## Code-Generated Images

JKnit will not automatically detect when a code chunk generates
an image (it calls an external program to receive the printed
output, how could it know if an image had been displayed?).
Thus, to display code-generated images, you must save them
locally in the generating code, and then use the saved filepath
to include the picture afterwards. For example, see the
following code.

\```language \
// Do some image processing here \
// Code which saves an image named ex.png \
\```

\!\[The image generated by our code\]\(ex.png\)\{width=50%\}

## Examples

More examples can be seen in `./demos`.

## License

This software is protected by the GPLv3.
