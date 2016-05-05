/*
Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


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
