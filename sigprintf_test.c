#include <unistd.h>

#include "sigprintf.h"

int
main(void)
{
	(void)sigputs("Hello, world");
	(void)sigfputs("Hello, world\n", STDERR_FILENO);

	return 0;
}
