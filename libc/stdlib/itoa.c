#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

char* itoa(int val,char *str, int base) 
{
	char *p = str;
	char *p1, *p2;
	uint64_t ud = val;

	int divisor = 10;

	if ((base == 'd' || base == 10) && val < 0)
	{
		*p++ =  '-';
		str++;
		ud = -val;
	}
	else if ( base == 'x' || base == 16)
	{
		divisor = 16;
	}

	do
	{
		int remainder = ud % divisor;
		*p++ = (remainder < 10 ) ? remainder + '0' : remainder + 'A' - 10;
	} while (ud /= divisor);

	*p = 0;

	p1 = str;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return p;
}
