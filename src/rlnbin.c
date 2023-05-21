#include "rln.h"
#include <stdlib.h>
#include <stdio.h>

int
main(argc, argv)
	int argc;
	char *argv[argc];
{
	char *prompt = ": ";
	size_t count = 128;
	
	if (argc > 1)
		prompt = argv[1];

	if (argc > 2)
			count = atoi(argv[2]);

	char *text = rln(prompt, count);
	setenv("RLNRET", text, 1);
	puts(text);
	free(text);
}
