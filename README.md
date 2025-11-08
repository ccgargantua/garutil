# garutil

A collection of utilities that I tend to copy from
project to project. All are self contained and do not
rely on anything other than the standard library. These
are compatible with C11.

## Utils

In the `util/` directory, you'll find a directory per
utility. Here is a list of the available utilities:

### [arena](https://github.com/ccgargantua/garutil/tree/main/util/arena)

The arena contains an incredibly basic arena allocator.
You provide the allocation, it distributes the pointers
and performs some sanity checks for you.

### [debug](https://github.com/ccgargantua/garutil/tree/main/util/debug)

The debug utility currently only contains a couple of
functions for printing debug messages to a log file.
By default, that log file is `stderr`, but that can be
changed if desired.

### [headers](https://github.com/ccgargantua/garutil/tree/main/util/headers)

Headers contains some useful headerfiles. Right now,
that includes some verbose assertions (purely stylistic,
I know) and some compiler extensions with conditional
compilation.

### [ini](https://github.com/ccgargantua/garutil/tree/main/util/ini)

ini contains a very, VERY simple ini file parser.
There is plenty about it that could be improved, sure,
but ini files are simple for the sake of being simple
to parse :)