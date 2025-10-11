#include <string.h>

#include <unistd.h>

#include "sigprintf.h"
#include "sigprintf_lex.h"

static int
format_to_buffer(char *b, const char *format, va_list ap)
{
	struct fmt_tok tok_list[24];
	int tok_c = 0;
	int i;
	char *start = (char *)format;
	char *ch;
	char f;
	long end;
	size_t fmt_len = strlen(format);
	ssize_t c = 0;

	(void)memset(b, 0, SIGPRINTF_FORMAT_LEN);
	while ((ch = strchr(start, '%')) != NULL)
	{
		end = ch - start;
		if (end > 0)
		{
			tok_list[tok_c].type = LITERAL;
			tok_list[tok_c].data = start;
			tok_list[tok_c].end = end;
			tok_c++;
		}
		f = *(ch + 1);
		switch (f)
		{
			case 'd':
				tok_list[tok_c].type = INT;
				start = ch + 2;
				break;
			case 'l':
				if (*(ch + 2) == 'u')
				{
					tok_list[tok_c].type = ULONG;
					start = ch + 3;
				}
				else
				{
					tok_list[tok_c].type = LONG;
					start = ch + 2;
				}
				break;
			case 'u':
				tok_list[tok_c].type = UINT;
				start = ch + 2;
				break;
			case 's':
				tok_list[tok_c].type = STRING;
				start = ch + 2;
				break;
			case '%':
				tok_list[tok_c].type = PERCENT;
				tok_list[tok_c].data = NULL;
				start = ch + 2;
				break;
			default:
				return -1;
		}
		if (tok_list[tok_c].type != PERCENT)
			tok_list[tok_c].data = va_arg(ap, void *);
		tok_c++;
	}
	if (start < format + fmt_len)
	{
		tok_list[tok_c].type = LITERAL;
		tok_list[tok_c].data = start;
		tok_list[tok_c].end = format + fmt_len - start;
		tok_c++;
	}

	return 0;
}

int
sigdputs(const char *s, int fd)
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

	c = sigdputs(s, STDIN_FILENO);
	if (c == -1) return -1;

	if (write(1, "\n", 1) == -1) return c;

	return c + 1;
}

int
sigvdprintf(int fd, const char *format, va_list ap)
{
	char s[SIGPRINTF_FORMAT_LEN];
	ssize_t c;

	format_to_buffer(s, format, ap);
	c = write(fd, s, strlen(s));
	if (c == -1) return -1;

	return (int)c;
}

int
sigdprintf(int fd, const char *format, ...)
{
	va_list ap;
	ssize_t c;

	va_start(ap, format);
	c = sigvdprintf(fd, format, ap);
	va_end(ap);
	if (c == -1) return -1;

	return (int)c;
}

int
sigprintf(const char *format, ...)
{
	va_list ap;
	ssize_t c;

	va_start(ap, format);
	c = sigvdprintf(STDOUT_FILENO, format, ap);
	va_end(ap);
	if (c == -1) return -1;

	return (int)c;
}
