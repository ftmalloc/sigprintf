#include <limits.h>

#include <unistd.h>

#include "sigprintf.h"
#include "sigprintf_conv.h"
#include "sigprintf_lex.h"
#include "sigstring.h"

#define SIGPRINTF_MAX_TOKENS 24

static int
format_to_buffer(char *b, const char *format, va_list ap)
{
	struct fmt_tok tok_list[SIGPRINTF_MAX_TOKENS];
	int tok_c = 0;
	int i;
	char *start = (char *)format;
	char *ch;
	char f;
	long end;
	char ltoa_buff[LTOA_BUFF_LEN];
	size_t fmt_len = sigstrlen(format);
	size_t c = 0;

	(void)sigmemset(b, 0, SIGPRINTF_FORMAT_LEN);
	while ((ch = sigstrchr(start, '%')) != NULL)
	{
		end = ch - start;
		if (end > 0)
		{
			tok_list[tok_c].type = LITERAL;
			tok_list[tok_c].data.lit.str = start;
			tok_list[tok_c].data.lit.len = end;
			tok_c++;
		}
		if (ch - format >= SIGPRINTF_FORMAT_LEN) return -1;

		f = *(ch + 1);
		switch (f)
		{
			case 'd':
				tok_list[tok_c].type = INT;
				tok_list[tok_c].data.d = va_arg(ap, int);
				start = ch + 2;
				break;
			case 'l':
				if (*(ch + 2) == 'u')
				{
					tok_list[tok_c].type = ULONG;
					tok_list[tok_c].data.l = va_arg(ap, unsigned long);
				}
				else if (*(ch + 2) == 'd')
				{
					tok_list[tok_c].type = LONG;
					tok_list[tok_c].data.ul = va_arg(ap, long);
				}
				else if (*(ch + 2) == 'x')
				{
					tok_list[tok_c].type = LHEX;
					tok_list[tok_c].data.ul = va_arg(ap, unsigned long);
				}
				else
				{
					return -1;
				}
				start = ch + 3;
				break;
			case 'u':
				tok_list[tok_c].type = UINT;
				tok_list[tok_c].data.u = va_arg(ap, unsigned int);
				start = ch + 2;
				break;
			case 'x':
				tok_list[tok_c].type = HEX;
				tok_list[tok_c].data.u = va_arg(ap, unsigned int);
				start = ch + 2;
				break;
			case 'p':
				tok_list[tok_c].type = POINTER;
				tok_list[tok_c].data.p = va_arg(ap, void *);
				start = ch + 2;
				break;
			case 'c':
				tok_list[tok_c].type = CHAR;
				tok_list[tok_c].data.c = va_arg(ap, int);
				start = ch + 2;
				break;
			case 's':
				tok_list[tok_c].type = STRING;
				tok_list[tok_c].data.s = va_arg(ap, char *);
				start = ch + 2;
				break;
			case '%':
				tok_list[tok_c].type = PERCENT;
				start = ch + 2;
				break;
			default:
				return -1;
		}
		tok_c++;
		if (tok_c >= SIGPRINTF_MAX_TOKENS) return -1;
	}
	if (start < format + fmt_len)
	{
		tok_list[tok_c].type = LITERAL;
		tok_list[tok_c].data.lit.str = start;
		tok_list[tok_c].data.lit.len = format + fmt_len - start;
		tok_c++;
		if (tok_c >= SIGPRINTF_MAX_TOKENS) return -1;
	}
	for (i = 0; i < tok_c; i++)
	{
		switch (tok_list[i].type)
		{
			case LITERAL:
				if (c + tok_list[i].data.lit.len >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigstrncpy(b + c, tok_list[i].data.lit.str, tok_list[i].data.lit.len);
				c += tok_list[i].data.lit.len;
				break;
			case INT:
				sigltoa(ltoa_buff, tok_list[i].data.d);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case LONG:
				sigltoa(ltoa_buff, tok_list[i].data.l);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case UINT:
				sigultoa(ltoa_buff, tok_list[i].data.u);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case ULONG:
				sigultoa(ltoa_buff, tok_list[i].data.ul);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case HEX:
				sightoa(ltoa_buff, tok_list[i].data.u);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case LHEX:
				sightoa(ltoa_buff, tok_list[i].data.ul);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case POINTER:
				sigptoa(ltoa_buff, tok_list[i].data.p);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
				break;
			case CHAR:
				if (c + 1 >= SIGPRINTF_FORMAT_LEN) return -1;

				b[c++] = tok_list[i].data.c;
				break;
			case STRING:
				if (c + sigstrlen(tok_list[i].data.s) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, tok_list[i].data.s, sigstrlen(tok_list[i].data.s));
				c += sigstrlen(tok_list[i].data.s);
				break;
			case PERCENT:
				if (c + 1 >= SIGPRINTF_FORMAT_LEN) return -1;

				b[c++] = '%';
				break;
		}
	}

	return 0;
}

int
sigdputs(const char *s, int fd)
{
	ssize_t c;

	c = write(fd, s, sigstrlen(s));
	if (c == -1) return -1;

	return (int)c;
}

int
sigputs(const char *s)
{
	int c;

	c = sigdputs(s, STDOUT_FILENO);
	if (c == -1) return -1;

	if (write(1, "\n", 1) == -1) return c;

	return c + 1;
}

int
sigvdprintf(int fd, const char *format, va_list ap)
{
	char s[SIGPRINTF_FORMAT_LEN];
	ssize_t c;

	if (format == NULL) return -1;

	if (format_to_buffer(s, format, ap) < 0) return -1;

	c = write(fd, s, sigstrlen(s));
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
