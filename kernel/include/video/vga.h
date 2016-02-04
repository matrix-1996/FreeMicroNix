#ifndef INCLUDE_VIDEO_VGA_H
#define INCLUDE_VIDEO_VGA_H
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
static inline unsigned char make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

/* Takes in the ascii value of the 
entered character, and the color that
the character should appear as on the 
screen. It does a logical OR on the 2
values. The result of the operation is
bitwise shifted over 8 place values.
The resulting value is the raw binary
representation of the color and ascii 
value */
static inline unsigned short make_vgaentry(char c, unsigned char color)
{
	unsigned short c16 = c;
	unsigned short color16 = color;
	return c16 | color16 << 8;
}

static const unsigned int VGA_WIDTH = 80; // The standard height of a vga terminal
static const unsigned int VGA_HEIGHT = 25; // The standard width of a vga terminal


/* The address of the VGA Port's
memory is set to 0xB8000 */
static unsigned short* const VGA_MEMORY = (unsigned short*) 0xB8000;

unsigned short detect_bios_area_hardware(void);
enum video_type get_bios_area_video_type(void);
void init_terminal(void);
void set_terminal_color(enum vga_color color);
void terminal_raw_putchar(char c, unsigned char color, unsigned int x, unsigned int y);
void move_terminal_cursor(void);
void clear_terminal(void);
void scroll_terminal(void);
void terminal_putchar(char c);
void terminal_write(const char* data, unsigned int size);
void terminal_writestring(const char* data);
void terminal_write_hex(unsigned int n);
void terminal_write_decimal(unsigned int n);

#endif