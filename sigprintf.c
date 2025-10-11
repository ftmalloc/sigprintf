#include <string.h>

#include <unistd.h>

#include "sigprintf.h"

int
sigfputs(const char *s, int fd)
{
	ssize_t c;

	c = write(fd, s, strlen(s));
	if (c == -1) return -1;

	return (int)c;
}

int
sigputs(const char *s)
{
	int c;

	c = sigfputs(s, STDIN_FILENO);
	if (c == -1) return -1;

	if (write(1, "\n", 1) == -1) return c;

	return c + 1;
}
