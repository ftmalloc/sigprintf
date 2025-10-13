#include "sigstring.h"

size_t
sigstrlen(const char *s)
{
	size_t i;

	for (i = 0; s[i] != '\0'; i++)
		;

	return i;
}

char *
sigstrncpy(char *dst, const char *src, size_t len)
{
	size_t i;

	for (i = 0; i < len && src[i] != '\0'; i++)
		dst[i] = src[i];
	for (; i < len; i++)
		dst[i] = '\0';

	return dst;
}

char *
sigstrchr(const char *s, int c)
{
	char ch = (char)c;

	do
	{
		if (*s == ch) return (char *)s;
	} while (*s++ != '\0');

	return NULL;
}

void *
sigmemset(void *s, int c, size_t n)
{
	char ch = (char)c;
	size_t i;

	for (i = 0; i < n; i++)
		((unsigned char *)s)[i] = ch;

	return s;
}

void *
sigmemcpy(void *dst, const void *src, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];

	return dst;
}

void *
sigmemmove(void *dst, const void *src, size_t n)
{
	unsigned char *d = dst;
	const unsigned char *s = src;
	size_t i;

	if (d == s || n == 0) return dst;

	if (d < s)
		for (i = 0; i < n; i++) d[i] = s[i];
	else
		for (i = n; i > 0; i--) d[i - 1] = s[i - 1];

	return dst;
}
