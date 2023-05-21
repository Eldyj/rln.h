#ifndef RLN_C
#	define RLN_C

#	include <termios.h>
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <cstr.h>
#	include "rln.h"

char
*rln(prompt, len)
	const char *const prompt;
	const size_t len;
{
	const size_t pl = cstr_len(prompt);
	struct termios orig_termios;
	tcgetattr(STDIN_FILENO, &orig_termios);
	struct termios raw_termios = orig_termios;
	raw_termios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_termios);
	char *line = malloc(sizeof(char) * 1);
	*line = 0;
	size_t index = 0;
	size_t max = 0;
	char c = 0;
	goto pr;
	
	while (1) {
		read(0, &c, 1);

		switch (c) {
			case '\n': { // end
				goto done;
				break;
			}

			case 'H': { // left
				if (index)
					--index;

				break;
			}

			case 'L': { // right
				if (index < max)
					++index;
					
				break;
			}

			case 'J': { // to start
				index = 0;
				break;
			}

			case 'K': { // to end
				index = max;
				break;
			}

			case 'U': { // rm before
				if (!index)
					continue;
				
				cstr_rm(&line, index);
				--max;
				--index;
				break;
			}

			case 'I': { // rm after
				if (index >= max)
					continue;

				cstr_rm(&line, index+1);
				--max;
				break;
			}

			case 'A': { // rm start
				if (max) {
					cstr_rm(&line, 1);
					--max;

					if (index) {
						--index;
					}
				}
				
				break;
			}

			case 'S': { // rm end
				if (max) {
					cstr_rm(&line, max);
					--max;
				}
				
				break;
			}

			case 'Z': { //clear before
				while (index) {
					cstr_rm(&line, index);
					--index;
					--max;
				}

				break;
			}

			case 'X': { //clear after
				while (max > index) {
					cstr_rm(&line, index+1);
					--max;
				}

				break;
			}

			case 'C': { // clear all
				max = 0;
				index = 0;
				free(line);
				line = malloc(sizeof(char) * 1);
				*line = 0;
				break;
			}

			case 'N': { //next word
				index = cstr_find_space_after(line, index);
				break;
			}

			case 'B': { //prev word
				index = cstr_find_space_before(line, index);
				break;
			}
		
			default: {
				if (max < len) {
					cstr_insert(&line, c, index);
					++index;
					++max;
				}
				break;
			}
		}
		pr:
		printf("\r\033[K%s%s\033[%zuG", prompt, line, index+pl+1);
		fflush(stdout);
	}

	done:
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	write(1, "\n", 1);
	return line;
}

void
rln_to(buf, prompt, len)
	char *const *const buf;
	const char *const prompt;
	const size_t len;
{
	char *t = rln(prompt, len);
	cstr_copy(buf, t);
	free(t);
}

#endif /* !RLN_C */
