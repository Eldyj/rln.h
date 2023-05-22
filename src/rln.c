#ifndef RLN_C
#	define RLN_C

#	include <termios.h>
#	include <unistd.h>
#	include <stdio.h>
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
	char *line = cstr_alloc(0);
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
				
				cstr_rem(&line, index-1);
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

							cstr_rem(&line, index);
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
			
			case 0x6: { // rm start; ctrl+f
				if (max) {
					cstr_rem(&line, 0);
					--max;

					if (index) {
						--index;
					}
				}
				
				break;
			}

			case 0x7: { // rm end; ctrl+g
				if (max) {
					cstr_rem(&line, max-1);
					--max;
				}

				if (max < index)
					index = max;
				
				break;
			}

			case 0xB: { //clear before ctrl+k
				if (index == max)
					goto clear;
				
				cstr_lshift(&line, index);
				max -= index;
				index = 0;
				break;
			}

			case 0xC: { //clear after ctrl+l
				if (!index)
					goto clear;

				cstr_rshift(&line, max-index);
				max = index;
				break;
			}

			case 0xF: { // clear all ctrl+o
				clear:
				cstr_free(line);
				line = cstr_alloc(0);
				*line = index = max = 0;
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
					cstr_put(&line, c, index);
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
	char *const buf;
	const char *const prompt;
	const size_t len;
{
	char *t = rln(prompt, len);
	cstr_copy(t, buf);
	cstr_free(t);
}

#endif /* !RLN_C */
