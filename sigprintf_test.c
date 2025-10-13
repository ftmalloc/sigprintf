#include <unistd.h>

#include "sigprintf.h"

int
main(void)
{
	(void)sigputs("Hello, world");
	(void)sigdputs("Hello, error world\n", STDERR_FILENO);
	(void)sigprintf("Hello %%%d %s The end!\n", -133, "world!");
	(void)sigdprintf(2, "Hello %%%d %s The err!\n", 1334, "world!");
	(void)sigprintf("This should be zero: %d\n", 0);
	(void)sigprintf(NULL);
	(void)sigprintf("Main function location: %p\n", (void *)main);

	return 0;
}
