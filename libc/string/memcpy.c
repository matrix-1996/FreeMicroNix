#include <string.h>

void *memcpy(void *dest, const void *src, size_t n) {
	char *dp = (char*)dest;
	const char *sp = (const char*)src;
	while (n--)
		*dp++ = *sp++;
	return dest;
}