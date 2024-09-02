# JKnit
## Jordan "Jorb" Dehmel, 2023 - Present
## jdehmel@outlook.com, github.com/jorbDehmel/jknit

JKnit is an extremely lightweight mathematical/computational document
knitting tool. It is purely FOSS forever, and aims to intuitive, easy and fast.

It compiles intermediary languages into .tex files. This allows running
code within good looking, feature-rich documents.

JKnit can only translate .jmd-like (.rmd, .md) files to .tex, but .pdf files
are very easy to generate from .tex. For linux, use `pdflatex`. For windows,
you can easily find an online converter.

JKnit works with the following languages by default.
Format: Language - command

Interpreted languages:
- Python3 - python
- Octave - octave

Compiled (non-knit) languages:
- C++ - clang++*
- C++ - gcc*
- Rust - rust*

`.rpres` formatting is dubiously supported as of version $0.0.11$.
You can activate this mode by changing a file's extension to `.rpres`
or `.jpres` (preferred). All setting are ignored as of this version.

### Installation requirements

These are actually just suggestions, but you will find that JKnit will have
little function without them.

 1 - Python
 2 - Octave (MatLab alternative)
 3 - Cargo (for Rust)
 4 - Nodejs (for JavaScript)
 5 - Clang (for C++, via LLVM)
 6 - MikTex or a comparable LaTeX compiler

All but the last of these items allow JKnit to compile and/or run their
respective languages. The final one, however, is used to translate JKnit's
output `.tex` files into more useful formats like `.pdf`.

### Linux Installation

Simply run the command `make install` while in this directory. Similarly,
run `make uninstall` to uninstall it. Currently, JKnit takes up less than 1
megabyte when installed on a Linux machine. No such spacial guarantees are
made for Windows.

### Windows Installation

Currently, all you need to do for Windows installation is run `gui.py` from IDLE.

JKnit is built for Linux, but does offer a less stable, less elegant implementation
for Windows. These files are found under the `experimental` directory. Note that
any and all executables used for JKnit will need to be run as admin (unlike Linux).
These files are designed to be built on Linux for Windows machines. As such,
pre-built binaries (.exe files) are included along with the source code.

### The CLI (Command Line Interface)

Command line flags and their meanings are shown below.

Tag   | Meaning
------|-------------
 \-o  | Set output file to the next argument
 \-l  | Enable log
 \-t  | Enable timer
 \-q  | Quiet (no printing)
 \-n  | No compile (halt before running)
 \-v  | Show version
 \-f  | From file (load settings)
 \-h  | Show help (this)

### Using Markdown

#### Headers

> \# Biggest Header

> \#\# Slightly Smaller Header

> \#\#\# Smaller

etc.

#### Bold text

> \*\*Bolded Text\*\*

#### Italics
> \*Italics\*

#### Lists

> \1 First Item
>
> \2 Second Item
>
> \- Unordered Item
>
> \- Another Item

#### Horizontal Line / Rule

\-\-\-\- (Any combinations of four or more dashes)

Note: Three dashes denotes a code section in .rmd, and will be ignored by
the .jmd compiler.

#### Links

> \[Text of link\]\(https://www.example.com\)

#### Images

> \!\[alt text\]\(image.jpg\)\{options\}

#### Blockquotes

> \> This text will be in a blockquote

#### Math

> \$\$
>
>     Math will go here, in LaTeX math format
>
> \$\$

#### Inline Math

Inline math will look like \$this\$.

#### Inline Code

Inline code will look like \`this\`.

#### LaTeX Commands

Most LaTeX commands can be used in any non-markdown formatted sections.
As of right now, using LaTeX commands are not supported within markdown
formatting (like lists).

### Loading Settings

By default, JKnit supports Python and Octave. However, it is trivial
to add support for any (interpreted) language. Simply include the
following code chunk at the top of your document.

> \`\`\`\{settings\}
>
> name command printChunkBreak extension
>
> \`\`\`

"name" is what you will put inside the curly brackets of future code
chunks in order to call the given command.

"printChunkBreak" is a command which will print the string "CHUNK_BREAK"
to the command line. This is **very important!** This is what will be used
to tell the chunks apart.

The "extension" is the file extension associated with that language.

For instance, the following code will add support for the cling C++
interpreter.

> \`\`\`\{settings\}
>
> cling cling cout<<"CHUNK_BREAK\n"; cpp
>
> \`\`\`

If you wish to include spaces in the command or printChunkBreak sections,
surround them in either single or double quotation marks.

## Build options

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

### The Lone Operator '\*'

Adding '\*' anywhere in your header command will cause a code chunk to
run without any others. For example,

> \`\`\`\{python\}\*
>
> a = "Hello!"
>
> \`\`\`

> \`\`\`\*\{python\}
>
> \# `a` will not be defined in this chunk!
>
> \```

If code chunk linking does not make sense for a language, you can include
the * operator in the language name itself as below.

> \`\`\`\{settings\}
>
> clang* clang++ ; cpp
>
> \`\`\`

> \`\`\`\{clang\}
>
> // Error: "Clang" is not defined
>
> \`\`\`

> \`\`\`\{clang\*\}
>
> // This will compile, but never link to any other clang* chunks.
>
> \`\`\`

### The Silent Operator '^'

If a code chunk produces no output, it is useful to mark is as such
using the ^ operator. For example,

> \`\`\`\{python\}^
>
> a = "Hello!"
>
> \`\`\`

> \`\`\`\{python\}
>
> print(a)
>
> \`\`\`

This code will effectively be run along with the next non-silent chunk.
Using "\*\^" would have no effect that using "\*" would not.

## Languages

#### Compiled Languages

Compiled languages must have a supplemental script written for them. This
script must take a filepath as an input and echo any printed text.

For instance, to implement truly compiled C++ code, you would need to
write a supplemental script that does the following.

 1. Take the .cpp file as a command line argument
 2. Compile the file
 3. Run the executable and echo its output

Several such "compilation drivers" are included in the compilation-drivers
directory. Make sure to save these files, as they are not copied anywhere
during installation (to save space). You can use these as below.

For C++ code via clang:

> \`\`\`\{clang++*\}
>
> // C++ code here
>
> \`\`\`

For Rust code via rustc:

> \`\`\`\{rust*\}
>
> // Rust code here
>
> \`\`\`

Since a useful compiled chunk will include a main function and only one
main function can be compiled, it is mostly useful to use these with the
'\*' lone operator, which makes a chunk run disconnected from any
other. This will cause jknit not to insert any instances of the chunk-break
print line, which will usually cause compilation failures in such languages.

#### Code-Generated Images

JKnit will not automatically detect when a code chunk generates an image
(it calls an external program to receive the printed output, how could it
know if an image had been displayed?). Thus, to display code-generated
images, you must save them locally in the generating code, and then use
the saved filepath to include the picture afterwards. For example, see the
following code.

> \`\`\`{example_language_name_here}
>
> // Do some image processing here
>
> // Code which saves an image file as example_1.bmp
>
> \`\`\`
>
> \!\[The .bmp image generated by our code\]\(example_1.bmp\)\{width=50%\}

## License

This software is protected by the GPLv3.
