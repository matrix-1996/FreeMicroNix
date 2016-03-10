#include <x86/video/vga.h>
#include <x86/io.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint32_t terminal_row;
uint32_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Returns a pointer the bios hardware information
  uint16_t detect_bios_area_hardware(void)
{
    const uint16_t* bda_detected_hardware_ptr = (const uint16_t*) 0x410;
    return *bda_detected_hardware_ptr;
}

// Returns the type of video hardwarea as defined in video/vga.h
enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) (detect_bios_area_hardware() & 0x30);
}

/*
This function initializes the terminal
It sets the position that text begins at to (0,0)
Then it checks for the type of video and sets the color accordingly
*/
void init_terminal(void)
{
	terminal_row = 0;
	terminal_column = 0;

	enum video_type terminal_videtotype = get_bios_area_video_type(); // Get the type of video device

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

	for ( uint32_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( uint32_t x = 0; x < VGA_WIDTH; x++ )
		{
			const uint32_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}

/*
Can be called with a vga_color enum value as a parameter
in order to change the color of the terminal text
*/
void set_terminal_color(enum vga_color color)
{
	terminal_color = color;
}

/* Calculates the position in the vga video buffer
to write to. Then it calls make_vgaentry */
void terminal_raw_putchar(char c, uint8_t color, uint32_t x, uint32_t y)
{
	const uint32_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void move_terminal_cursor(void)
{
	uint32_t cursorLocation = terminal_row * 80 + terminal_column;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}


// Clears the screen, and moves the text position to (0,0)
void clear_terminal(void)
{
	for ( uint32_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( uint32_t x = 0; x < VGA_WIDTH; x++ )
		{
			terminal_raw_putchar(' ', terminal_color, x, y);
		}
	}
	terminal_row = 0;
	terminal_column = 0;
	move_terminal_cursor();	
}

/* Moves everything on the screen up a row
discarding the first row and clearing the
bottom row once what was there has been moved.
Then the current row is set to 1 less than VGA_HEIGHT,
and the current column is set to one */
void scroll_terminal(void)
{
	if  (terminal_column >= 25)
    {
        if (terminal_row >= VGA_HEIGHT)
        {
            uint32_t i;
            for (i = 0 *VGA_WIDTH; i < 24*VGA_WIDTH; i++)
            {
                terminal_buffer[i] = terminal_buffer[i + VGA_WIDTH];
            }

            for (i = 24 * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
            {
                terminal_buffer[i] = ' ';
            }

            terminal_row = 24;
        }
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
    // Handle any other kprintfable character.
    else if(c >= ' ')
    {
    	terminal_raw_putchar(c, terminal_color, terminal_column, terminal_row);
    	terminal_column++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (terminal_column >= VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row ++;
    }

    // Scroll the screen if needed.
    scroll_terminal();
    // Move the hardware cursor.
    move_terminal_cursor();

}


void terminal_write(const char* data, uint32_t size)
{
	for ( uint32_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}


void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}


void kprintf(const char *s, ...)
{
    va_list ap;

    char buf[16];
    int i, j, size, buflen, neg;

    uint8_t c;
    int ival;
    uint32_t uival;

    va_start(ap, s);

    while ((c = *s++)) {
        size = 0;
        neg = 0;

        if (c == 0)
            break;
        else if (c == '%') {
            c = *s++;
            if (c >= '0' && c <= '9') {
                size = c - '0';
                c = *s++;
            }

            if (c == 'd') {
                ival = va_arg(ap, int);
                if (ival < 0) {
                    uival = 0 - ival;
                    neg++;
                } else
                    uival = ival;
                itoa(uival, buf, 10);

                buflen = strlen(buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0;
                         i--, j--)
                        buf[i] =
                            (j >=
                             0) ? buf[j] : '0';

                if (neg)
                    kprintf("-%s", buf);
                else
                    kprintf(buf);
            }
             else if (c == 'u') {
                uival = va_arg(ap, int);
                itoa(uival, buf, 10);

                buflen = strlen(buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0;
                         i--, j--)
                        buf[i] =
                            (j >=
                             0) ? buf[j] : '0';

                kprintf(buf);
            } else if (c == 'x' || c == 'X') {
                uival = va_arg(ap, int);
                itoa(uival, buf, 16);

                buflen = strlen(buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0;
                         i--, j--)
                        buf[i] =
                            (j >=
                             0) ? buf[j] : '0';

                kprintf("0x%s", buf);
            } else if (c == 'p') {
                uival = va_arg(ap, int);
                itoa(uival, buf, 16);
                size = 8;

                buflen = strlen(buf);
                if (buflen < size)
                    for (i = size, j = buflen; i >= 0;
                         i--, j--)
                        buf[i] =
                            (j >=
                             0) ? buf[j] : '0';

                kprintf("0x%s", buf);
            } else if (c == 's') {
                kprintf((char *) va_arg(ap, int));
            } 
        } else
            putchar(c);
    }

    return;
}
