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


char* u64toa(char* buf, unsigned long long val, int base, int min) {
	static char num[] = "0123456789ABCDEF";
	char* wstr=buf;
	
	// Validate base
	if (base<2 || base>16) {
		*wstr = '\0';
		return buf;
	}
	
	do {
		*wstr++ = num[(unsigned long long)val%base];
		if (min > 0)
			min--;
	} while (val/=base);

	while (min > 0) {
		*wstr++ = '0';
		min--;
	}
	*wstr = '\0';

	// Reverse string
	strreverse(buf);
	return buf;
}