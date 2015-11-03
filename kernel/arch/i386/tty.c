#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;


// Returns a pointer the bios hardware information
  uint16_t detect_bios_area_hardware(void)
{
    const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
    return *bda_detected_hardware_ptr;
}

// Returns the type of video hardwarea as defined in kernel/vga.h
enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) (detect_bios_area_hardware() & 0x30);
}

/*
This function initializes the terminal
It sets the position that text begins 
at to (0,0)
Then it checks for the type of video and sets the color accordingly
*/
void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 1;

	enum video_type terminal_videtotype = get_bios_area_video_type();
	if (terminal_videtotype == VIDEO_TYPE_MONOCHROME )
	{
		terminal_color = make_color(COLOR_BLACK, COLOR_BLACK);
	}
	else if (terminal_videtotype == terminal_videtotype)
	{
			terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	}
	else terminal_color = COLOR_LIGHT_GREY;
	
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

/* Calculates the position in the vga video buffer
to write to. Then it calls make_vgaentry */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}


// Clears the screen, and moves the text position to (0,0)
void terminal_clear(void)
{
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			terminal_putentryat(' ', terminal_color, x, y);
		}
	}
	terminal_row = 0;
	terminal_column = 0;	
}

/* Moves everything on the screen up a row
discarding the first row and clearing the
bottom row once what was there has been moved.
Then the current row is set to 1 less than VGA_HEIGHT,
and the current column is set to one */
void terminal_scroll(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y+1) * VGA_WIDTH + x];
		}
		for (size_t i = 0; i < VGA_WIDTH; i++)
		{
			terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT);
		}
		terminal_row = VGA_HEIGHT-1;
		terminal_column = 1;
	}

}

void terminal_putchar(char c)
{

	if (terminal_row >= VGA_HEIGHT)
	{
		terminal_scroll();
	}	

	if (c == '\t')
	{
		for (size_t i = 0; i < 4; i++)
		{
			terminal_putchar(' ');
		}
	}

	else if (c == '\n')
	{
		terminal_column = 0;
		terminal_row++;

	}

	else 
		{
			terminal_putentryat(c, terminal_color, terminal_column -1, terminal_row);
		}

	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row = 0;
		}
	}
}

void terminal_write_decimal(int d)
{
	terminal_putchar(itoa(d,10));
}

void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}
