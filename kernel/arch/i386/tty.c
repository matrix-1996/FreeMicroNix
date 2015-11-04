#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/common.h>
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


void terminal_move_cursor(void)
{
	size_t cursorLocation = terminal_row * 80 + terminal_column;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}



/* Moves everything on the screen up a row
discarding the first row and clearing the
bottom row once what was there has been moved.
Then the current row is set to 1 less than VGA_HEIGHT,
and the current column is set to one */
void terminal_scroll(void)
{
	if  (terminal_row >= 25)

	for (int y = 0; y < VGA_HEIGHT; y++)
	{
		for (int x = 0; x < VGA_WIDTH; x++)
		{
			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y+1) * VGA_WIDTH + x];
		}
		for (int i = 0; i < VGA_WIDTH; i++)
		{
			terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT);
		}
		terminal_row = VGA_HEIGHT-1;
		terminal_column = 1;
	}

}

void terminal_putchar(char c)
{

    // Handle a backspace, by moving the cursor back one space
    if (c == '\b' && terminal_column)
    {
        terminal_column--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == '\t')
    {
        terminal_column = (terminal_column+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        terminal_column = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        terminal_column = 0;
        terminal_row++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
    	terminal_putentryat(c, terminal_color, terminal_column -1, terminal_row);
    	terminal_column++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (terminal_column >= VGA_HEIGHT)
    {
        terminal_column = 0;
        terminal_row ++;
    }

    // Scroll the screen if needed.
    terminal_scroll();
    // Move the hardware cursor.
    terminal_move_cursor();

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


void terminal_write_hex(uint32_t n)
{
    int32_t tmp;

    terminal_writestring("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            terminal_putchar (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            terminal_putchar( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        terminal_putchar (tmp-0xA+'a');
    }
    else
    {
        terminal_putchar (tmp+'0');
    }

}



void terminal_write_decimal(uint32_t n)
{
	 if (n == 0)
    {
        terminal_putchar('0');
        return;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    terminal_writestring(c2);
}

