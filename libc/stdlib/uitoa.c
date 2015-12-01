#include <stdlib.h>

char* uitoa(char* buf, unsigned int val, int base, int min) {
	static char num[] = "0123456789ABCDEF";
	char* wstr=buf;
	
	// Validate base
	if (base<2 || base>16) {
		*wstr = '\0';
		return buf;
	}
	
	do {
		*wstr++ = num[(unsigned int)val%base];
		if (min > 0)
			min--;
	} while (val/=base);

	while (min > 0) {
		*wstr++ = '0';
		min--;
	}
	*wstr = '\0';

	// Reverse string
	strreverse(buf, wstr-1);
	return buf;
}