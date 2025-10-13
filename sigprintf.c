#include <unistd.h>

#include "sigprintf.h"
#include "sigprintf_lex.h"
#include "sigstring.h"

#define LTOA_BUFF_LEN 21 /* Max digits of an unsigned 64-bit integer */
#define SIGPRINTF_MAX_TOKENS 24

union
ptr_bytes
{
	void *p;
	unsigned char b[sizeof(void *)];
};

static void
sigultoa(char *b, unsigned long u)
{
	int d = 0;
	int i;
	size_t b_len;
	char temp;

	(void)sigmemset(b, 0, LTOA_BUFF_LEN);
	if (u == 0)
	{
		b[0] = '0';

		return;
	}
	while (u > 0)
	{
		b[d++] = (char)(u % 10 + '0');
		u /= 10;
	}
	b_len = sigstrlen(b);
	for (i = 0; i < b_len >> 1; i++)
	{
		temp = b[i];
		b[i] = b[b_len - 1 - i];
		b[b_len - 1 - i] = temp;
	}
}

static void
sigltoa(char *b, long d)
{
	long dabs;
	int neg;

	if (d < 0)
	{
		dabs = -d;
		neg = 1;
	}
	else
	{
		dabs = d;
		neg = 0;
	}
	sigultoa(b, dabs);
	if (neg)
	{
		(void)sigmemmove(&b[1], b, sigstrlen(b) + 1);
		b[0] = '-';
	}
}

static void
sigptoa(char *b, void *p)
{
	const char charset[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	union ptr_bytes pbytes;
	ssize_t plen = sizeof(void *);
	ssize_t i;
	size_t w;
	char h;
	char l;

	pbytes.p = p;
	b[0] = '0';
	b[1] = 'x';

	for (i = plen - 1, w = 2; i >= 0; i--)
	{
		h = charset[pbytes.b[i] >> 4 & 0xf];
		l = charset[pbytes.b[i] & 0xf];

		if (h == '0' && l == '0' && w == 2) continue;

		if (w != 2 || h != '0') b[w++] = h;
		b[w++] = l;
	}
	b[w] = '\0';
}

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
			case 'p':
				tok_list[tok_c].type = POINTER;
				tok_list[tok_c].data.p = va_arg(ap, void *);
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
			case POINTER:
				sigptoa(ltoa_buff, tok_list[i].data.p);
				if (c + sigstrlen(ltoa_buff) >= SIGPRINTF_FORMAT_LEN) return -1;

				(void)sigmemcpy(b + c, ltoa_buff, sigstrlen(ltoa_buff));
				c += sigstrlen(ltoa_buff);
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
