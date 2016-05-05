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
