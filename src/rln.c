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

			case 0x7F:
			case 0x08: { // rm before; backspace
				if (!index)
					continue;
				
				cstr_rm(&line, index);
				--max;
				--index;
				break;
			}

			case 27: {
				char seq[2];
				read(0, seq, 2);
				
				if (seq[0] == '[') {
					switch (seq[1]) {
						case 'D': { // left
							if (index)
								--index;

							break;
						}

						case 'C': { // right
							if (index < max)
								++index;
								
							break;
						}

						case 'A': { // up arrow
							break;
						}

						case 'B': { // down arrow
							break;
						}

						case '1': {
							read(1, &c, 1);

							if (c == ';') {
								read(1, seq, 2);

								if (seq[0] == '5') {
									switch (seq[1]) {
										case 'D': { // word left; ctrl+left
											index = cstr_find_space_before(line, index);
											break;
										}

										case 'C': { // word right; ctrl+right
											index = cstr_find_space_after(line, index);
											break;
										}
									}
								}
							}
							break;
						}

						case 'P': { // rm after; delete
							if (index >= max)
									continue;

							cstr_rm(&line, index+1);
							--max;
							break;
						}
					}
				}
				break;
			}

			case 0x1: { // goto start; home char
				index = 0;
				break;
			}

			case 0x4:   // goto end; ctrl+d
			case 0x5: { // goto end; end char
				index = max;
				break;
			}

			case 0x11: { // rm start ctrl+q
				if (max) {
					cstr_rm(&line, 1);
					--max;

					if (index) {
						--index;
					}
				}
				
				break;
			}

			case 0x23: { // rm end ctrl+w
				if (max) {
					cstr_rm(&line, max);
					--max;
				}
				
				break;
			}

			case 0xB: { //clear before ctrl+k
				while (index) {
					cstr_rm(&line, index);
					--index;
					--max;
				}

				break;
			}

			case 0xC: { //clear after ctrl+l
				while (max > index) {
					cstr_rm(&line, index+1);
					--max;
				}

				break;
			}

			case 0xF: { // clear all ctrl+o
				while (max > index) {
					cstr_rm(&line, index+1);
					--max;
				}

				while (index) {
					cstr_rm(&line, index);
					--index;
					--max;
				}

				break;
			}

			case 0x12: { // swap prev char; ctrl+r
				if (index > 1 && index < max) {
					const char tmp = line[index-1];
					line[index-1] = line[index-2];
					line[index-2] = tmp;
				}
				
				break;
			}

			case 0x14: { // swap next char; ctrl+t
				if (index && index < max) {
					const char tmp = line[index];
					line[index] = line[index-1];
					line[index-1] = tmp;
				}

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
		
		line[max-1] = 0;
		line[max] = 0;
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
