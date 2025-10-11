#ifndef SIGPRINTF_LIBRARY_H
#define SIGPRINTF_LIBRARY_H

#include <stdarg.h>

#define SIGPRINTF_FORMAT_LEN 1024

int sigdputs(const char *s, int fd);
int sigputs(const char *s);
int sigvdprintf(int fd, const char *format, va_list ap);
int sigdprintf(int fd, const char *format, ...);
int sigprintf(const char *format, ...);

#endif /* SIGPRINTF_LIBRARY_H */
