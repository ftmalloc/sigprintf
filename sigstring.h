#ifndef SIGPRINTF_SIGSTRING_H
#define SIGPRINTF_SIGSTRING_H

#include <stddef.h>

size_t sigstrlen(const char *s);
char *sigstrncpy(char *dst, const char *src, size_t len);
char *sigstrchr(const char *s, int c);
void *sigmemset(void *s, int c, size_t n);
void *sigmemcpy(void *dst, const void *src, size_t n);
void *sigmemmove(void *dst, const void *src, size_t n);

#endif /* SIGPRINTF_SIGSTRING_H */
