#include "../src/rln.h"
#include <stdio.h>
#include <stdlib.h>

int
main(void)
	/* VOID */
{
	char *s = rln("enter something: ");
	puts(s);
	free(s);
}
