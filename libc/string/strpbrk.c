#include <string.h>

char* strpbrk(const char * str, const char * accept) {
	const char *acc = accept;

	if (!*str) {
		return 0;
	}

	while (*str) {
		for (acc = accept; *acc; ++acc) {
			if (*str == *acc) {
				break;
			}
		}
		if (*acc) {
			break;
		}
		++str;
	}

	if (*acc == '\0') {
		return 0;
	}

	return (char *)str;
}