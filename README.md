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

| Format specifier | Description                              |
|------------------|------------------------------------------|
| %s               | Print a string                           |
| %p               | Print a pointer                          |
| %d               | Print a signed integer                   |
| %ld              | Print a signed long integer              |
| %u               | Print an unsigned integer                |
| %lu              | Print an unsigned long integer           |
| %%               | Print a literal '%'                      |

## Building

The included CMakeLists.txt file can be used to build sigprintf, typically as a static library for use in other projects.
The implementation is simple enough however that it should be easy to integrate into other build systems.
