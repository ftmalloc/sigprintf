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
	STRING,
	PERCENT
};

struct
fmt_tok
{
	enum fmt_type type;
	void *data;
	long end; /* only used for literals */
};

#endif /* SIGPRINTF_SIGPRINTF_LEX_H */
