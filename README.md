# sigprintf

sigprintf is a library implementing limited printing and formatting functions in a way that can be guaranteed safe in signal handlers.
It also includes a handful of guaranteed signal-safe string.h compatible functions.

sigprintf is written in C89 and targets POSIX systems.
It was originally written for use in ftmalloc, but can be used anywhere signal-safe format strings are desired.

## Usage

sigprintf includes the following functions declared in sigprintf.h:
```C
int sigdputs(const char *s, int fd);
int sigputs(const char *s);
int sigvdprintf(int fd, const char *format, va_list ap);
int sigdprintf(int fd, const char *format, ...);
int sigprintf(const char *format, ...);
```
Note that `sigdputs` is the sigprintf equivalent of `fputs`, but writes to a file descriptor instead of a stream.
To write to stderr, use:
```C
sigdputs(s, STDERR_FILENO);
```

The `sigvdprintf`, `sigdprintf`, and `sigprintf` functions accept a limited subset of format specifiers as described in the table below:

| Format specifier | Description                                     |
|------------------|-------------------------------------------------|
| %d               | Print a signed integer                          |
| %u               | Print an unsigned integer                       |
| %ld              | Print a signed long integer                     |
| %lu              | Print an unsigned long integer                  |
| %x               | Print an unsigned integer in hexadecimal        |
| %lx              | Print an unsigned long integer in hexadecimal   |
| %p               | Print a pointer                                 |
| %c               | Print a character                               |
| %s               | Print a string                                  |
| %%               | Print a literal '%'                             |

NOTE: These specifiers do not support width, precision, or length modifiers.

## Building

The included CMakeLists.txt file can be used to build sigprintf, typically as a static library for use in other projects.
The implementation is simple enough however that it should be easy to integrate into other build systems.

## Tests

The test suite uses RSpec, and so depends on Ruby and RSpec.
Assuming Ruby is installed on your system, RSpec can be installed with the following command:
```Bash
gem install rspec
```

With RSpec installed and added to PATH, and after making sure that the `sigprintf_test` target has been built, the test suite can be run by running `rspec` in the root of the project.

There is also a small driver application included, `sigprintf_driver`, which runs through some of the printing and formatting options that would be more difficult to unit test.
