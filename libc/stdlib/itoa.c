#include <stdlib.h>
#include <stdbool.h>

char* itoa(int val,char *str, int base) 
{
	int i = 0;
	bool isNegative = false;

	if (val == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	if (val < 0 && base == 10)
	{
		isNegative = true;
		val = -val;
	}

	while (val != 0)
	{
		int rem = val % base;
		str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
		val = val/base;
	}

	if(isNegative)
	{
		str[i++] = '-';
	}

	str[i] = '\0';
	strreverse(str);
	return str;
}
