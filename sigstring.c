#include "sigstring.h"

size_t
sigstrlen(const char *s)
{
	size_t i;

	if (s == NULL) return 0;

	for (i = 0; s[i] != '\0'; i++)
		;

	return i;
}

char *
sigstrncpy(char *dst, const char *src, size_t len)
{
	return dst;
}

char *
sigstrchr(const char *s, int c)
{
	return NULL;
}

void *
sigmemset(void *s, int c, size_t n)
{
	return s;
}

void *
sigmemcpy(void *dst, const void *src, size_t n)
{
	return dst;
}

void *
sigmemmove(void *dst, const void *src, size_t n)
{
	unsigned char b[MEMMOVE_BUFF_LEN];

	if (n >= MEMMOVE_BUFF_LEN) return NULL;

	(void)sigmemcpy(b, src, n);
	(void)sigmemcpy(dst, b, n);

	return dst;
}
