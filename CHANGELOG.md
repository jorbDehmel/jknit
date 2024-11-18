# Changelog of JKnit
Jordan Dehmel, 2024 - present

# `0.1.4`
- Added `-C` and `-x` CLI flags
- Added `forceFormalFont` option, which occurs if you use `-xx`.
    This will use the default `tex` font, rather than the
    informal one usually used.
- Fixed bug preventing paragraph breaks from working when
    targetting LaTeX

# `0.1.3`
- Switched from GPLv3 to MIT on the latest rewrite. This was
    originally the intent with the rewrite, and the continued
    inclusion of the GPL was a mistake. I believe that few, if
    any, distributions of the rewrite currently exist. This may
    or may not cause licensing issues in the future.
- Dropped experimental presentation support; After converting
    `jmd` to `md`, use a tool like `marp` to convert
    presentations

# `0.1.2`
- Added `debug-install` option, with default being stripped
    executable. Previously debug was default.
- Fixed issue with `tex` headers being wrong font

# `0.1.1`
- Added support for raw commands in headers
- Header commands will now have `%` replaced with the source
    file

# `0.1.0`
- Full rewrite to improve code quality
- Can now target `md` or `tex`, made it fairly easy to add new
    target languages
- Debugging compilation driver scripts
- Dropped Windows support

# `0.0.13`
- Began changelog
- Fixed stupid bug in latex header (from find and replace
    `string` -> `std::string`)
- Added `.clang-format`
- Various minor style changes
- Fixed names of compilation drivers, added one for `Oak`
