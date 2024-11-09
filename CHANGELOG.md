# Changelog of JKnit
Jordan Dehmel, 2024 - present

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
