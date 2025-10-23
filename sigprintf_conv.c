#include <limits.h>

#include <sys/types.h>

#include "sigstring.h"
#include "sigprintf_conv.h"

#define SIGPRINTF_LITTLE_ENDIAN 1

static int
endian_test(void)
{
	int i = 1;
	char *c = (char *)&i;

	return *c;
}

void
sigultoa(char *b, unsigned long ul)
{
	int d = 0;
	int i;
	char temp;

	if (ul == 0)
	{
		b[0] = '0';
		b[1] = '\0';

		return;
	}
	while (ul > 0)
	{
		b[d++] = (char)(ul % 10 + '0');
		ul /= 10;
	}
	for (i = 0; i < d >> 1; i++)
	{
		temp = b[i];
		b[i] = b[d - 1 - i];
		b[d - 1 - i] = temp;
	}
	b[d] = '\0';
}

void
sigltoa(char *b, long d)
{
	long dabs;
	int neg;
	int min = 0;
	size_t i;

	if (d < 0)
	{
		if (d == LONG_MIN)
		{
			dabs = LONG_MAX;
			min = 1;
		}
		else
		{
			dabs = -d;
		}
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
		if (min)
			for (i = sigstrlen(b) - 1; i > 0; i--)
			{
				if (++b[i] == '9' + 1)
					b[i] = '0';
				else
					break;
			}
	}
}

void
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

	if (endian_test() == SIGPRINTF_LITTLE_ENDIAN)
	{
		for (i = plen - 1, w = 2; i >= 0; i--)
		{
			h = charset[pbytes.b[i] >> 4 & 0xf];
			l = charset[pbytes.b[i] & 0xf];

			if (h == '0' && l == '0' && w == 2) continue;

			if (w != 2 || h != '0') b[w++] = h;
			b[w++] = l;
		}
	}
	else /* Big Endian */
	{
		for (i = 0, w = 2; i < plen; i++)
		{
			h = charset[pbytes.b[i] >> 4 & 0xf];
			l = charset[pbytes.b[i] & 0xf];

			if (h == '0' && l == '0' && w == 2) continue;

			if (w != 2 || h != '0') b[w++] = h;
			b[w++] = l;
		}
	}
	if (w == 2) b[w++] = '0';
	b[w] = '\0';
}

void
sightoa(char *b, unsigned long ul)
{
	const char charset[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	ssize_t i;
	size_t w;
	char c;

	b[0] = '0';
	b[1] = 'x';
	if (ul == 0)
	{
		b[2] = '0';
		b[3] = '\0';

		return;
	}

	for (i = sizeof(unsigned long) * 8 - 4, w = 2; i >= 0; i -= 4)
	{
		c = charset[ul >> i & 0xf];

		if (c == '0' && w == 2) continue;
		b[w++] = c;
	}
	b[w] = '\0';
}
