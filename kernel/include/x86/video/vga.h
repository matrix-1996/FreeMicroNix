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

#ifndef _X86_VIDEO_VGA_H
#define _X86_VIDEO_VGA_H
#include <stdint.h>
#include <stddef.h>

enum video_type
{
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOUR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30,
};

enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

/* Takes in the desired foreground
and background color and does a logical 
OR of the two values. Then the 
resulting value is bitwise shifted to
the left 4 place values. The resulting
value is the raw binary representation
of the combination of foreground and
background colors. */
static inline uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}


static const uint32_t VGA_WIDTH = 80; // The standard height of a vga terminal
static const uint32_t VGA_HEIGHT = 25; // The standard width of a vga terminal


/* The address of the VGA Port's
memory is set to 0xB8000 */
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

void Terminal_Set_Color(enum vga_color color);
void Initialize_Terminal(void);
void Terminal_Scroll(void);
void Terminal_Putat(uint32_t, uint32_t y, char c);
void Terminal_Putch(char c);
void Terminal_Print(char *s);
void Terminal_Print_Int(int32_t i);
void Terminal_Println(char* s);
void Terminal_Backspace(void);
void Terminal_Newline(void);
void Terminal(void);
void kprintf(const char *format, ...);
void Terminal_Update_Cursor(void);

#endif