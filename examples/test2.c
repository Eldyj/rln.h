#include "../src/rln.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstr.h>

int
main(void)
	/* VOID */
{
	char *buf = malloc(sizeof(char) * 101);

	while (1) {
		rln_to(buf, "enter something: ", 100);

		if (cstr_equals(buf, "exit")) {
			break;
		}

		puts(buf);		
	}

	free(buf);
}
