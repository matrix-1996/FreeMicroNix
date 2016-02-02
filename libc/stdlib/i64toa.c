#include <stdlib.h>

char* i64toa(char* buf, long long val, int base, int min) {
	static char num[] = "0123456789ABCDEF";
	char* wstr=buf;
	int sign;
	unsigned long long uval = val;
	
	// Validate base
	if (base<2 || base>16){ *wstr='\0'; return buf; }
	
	if (uval > 0x7FFFFFFF && base == 16) {

		do {
			*wstr++ = num[(unsigned long long)uval%base];
			if (min > 0)
				min--;
		} while (uval/=base);
	
		while (min > 0) {
			*wstr++ = '0';
			min--;
		}
		*wstr='\0';
	} else {
		// Take care of sign
		if ((sign=val) < 0) val = -val;
	
		// Conversion. Number is reversed.
		do {
			*wstr++ = num[(unsigned long long)val%base];
			if (min > 0)
				min--;
		} while (val/=base);
	
		while (min > 0) {
			*wstr++ = '0';
			min--;
		}

		if(sign < 0 && base == 10) *wstr++='-';
		*wstr='\0';
	}
	// Reverse string
	strreverse(buf,wstr-1);
	return buf;
}