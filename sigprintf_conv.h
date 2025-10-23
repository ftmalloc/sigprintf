#ifndef SIGPRINTF_SIGPRINTF_CONV_H
#define SIGPRINTF_SIGPRINTF_CONV_H

#define SPECIFIER_BUFF_LEN 21 /* Max digits of an unsigned 64-bit integer */

union
ptr_bytes
{
	void *p;
	unsigned char b[sizeof(void *)];
};

void sigultoa(char *, unsigned long);
void sigltoa(char *, long);
void sigptoa(char *, void *);
void sightoa(char *, unsigned long);

#endif /* SIGPRINTF_SIGPRINTF_CONV_H */