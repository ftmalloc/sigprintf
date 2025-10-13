#ifndef SIGPRINTF_SIGPRINTF_LEX_H
#define SIGPRINTF_SIGPRINTF_LEX_H

enum
fmt_type
{
	LITERAL,
	INT,
	UINT,
	LONG,
	ULONG,
	POINTER,
	STRING,
	PERCENT
};

struct
fmt_tok
{
	enum fmt_type type;
	union
	{
		struct
		{
			char *str;
			size_t len;
		} lit;
		int d;
		unsigned int u;
		long l;
		unsigned long ul;
		void *p;
		char *s;

	} data;
};

#endif /* SIGPRINTF_SIGPRINTF_LEX_H */
