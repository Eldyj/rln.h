#ifndef RLN_H
#	define RLN_H
#	include <stddef.h>

char *rln(const char *const prompt, const size_t len);
void rln_to(char *const *const buf, const char *prompt, const size_t len);

#endif /* RLN_H */
