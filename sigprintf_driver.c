#include <limits.h>

#include <unistd.h>

#include "sigprintf.h"

int
main(void)
{
	(void)sigputs("Hello, world!");
	(void)sigdputs("Hello, error world!\n", STDERR_FILENO);
	(void)sigprintf("%s %s %s\n", "Hello", "printf", "world");
	(void)sigprintf("The value of LONG_MIN on this system is: %ld\n", LONG_MIN);
	(void)sigprintf("The value of INT_MIN on this system is: %d\n", INT_MIN);
	(void)sigprintf("The value of ULONG_MAX on this system is: %lu\n", ULONG_MAX);
	(void)sigprintf("Here's a single character: %c\n", 'h');
	(void)sigprintf(NULL);

	return 0;
}
