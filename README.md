# JKnit
## Jordan "Jorb" Dehmel, 2023 - Present
## jdehmel@outlook.com, github.com/jorbDehmel/jknit

JKnit is an extremely lightweight mathematical/computational document
knitting tool. It is purely FOSS forever, and aims to intuitive, easy and fast.

It compiles intermediary languages into .tex files. This allows running
code within good looking, feature-rich documents.

### .jks Files and Loading Settings

.jks (JKnit Settings) files hold the interpreter paths of JKnit's
languages. A line of a .jks file reads like this:

> 'name' 'path/to/executable' '.file_extension' 'command line args go here'

This is loaded by the program. This can also go in a {settings} chunk.

### .jmd Files

.jmd files are mostly LaTeX, with chunks of code throughout. A code chunk
is denoted as follows.

Things out here are not code.

\```{name}

print('Hello, world!')

\```

This end isn't code either.

The code will appear in the compiled document, as well as its output.
Most basic markdown syntax will work.

### License

This software is protected by the GPLv3.
