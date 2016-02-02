#include <stdlib.h>

int atoi(const char * str) {
	unsigned int len = strlen(str);
	unsigned int out = 0;
	unsigned int i;
	unsigned int pow = 1;
	for (i = len; i > 0; --i) {
		out += (str[i-1] - 48) * pow;
		pow *= 10;
	}
	return out;
}
