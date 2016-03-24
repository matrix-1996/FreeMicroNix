#include <x86/video/vga.h>
#include <x86/io.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

uint32_t terminal_row;
uint32_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

/*
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
*/


void Initialize_Terminal(void)
{
	terminal_row = 0;
	terminal_column = 0;

	Terminal_Set_Color(COLOR_LIGHT_GREY);
	
	terminal_buffer = VGA_MEMORY;

	for ( uint32_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( uint32_t x = 0; x < VGA_WIDTH; x++ )
		{
            Terminal_Putat(x, y, ' ');
		}
	}
}


void Terminal_Scroll(void)
{
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
        {
            terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
}

void Terminal_Putat(uint32_t x, uint32_t y, char c)
{
    const uint32_t index = y * VGA_WIDTH + x;
    uint16_t color16 = terminal_color;
    terminal_buffer[index] = c | color16 << 8;
}


void Terminal_Putch(char c)
{
    if (c == '\n' || terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        terminal_row++;
        return;
    }
    else if (c == '\t')
    {
        terminal_column = (terminal_column+8) & ~(8-1);
    }
    else if (c == '\b')
    {
        Terminal_Backspace();
    }
    else if (c == '\r')
    {
        terminal_row = 0;
    }

    if (terminal_row >= VGA_HEIGHT)
    {
        Terminal_Scroll();
    }

    Terminal_Putat(terminal_column, terminal_row, c);
    terminal_column++;
}

void Terminal_Print(char* s)
{
    uint32_t len = strlen(s);
    for (uint32_t i = 0; i < len; i++)
    {
        Terminal_Putch(s[i]);
    }
    Terminal_Update_Cursor();
}

void Terminal_Print_Int(int32_t i)
{
    char *s = (char*)"\0";
    itoa(i, s, 16);
    Terminal_Print(s);
}

void Terminal_Println(char *s)
{
    Terminal_Print(s);
    Terminal_Putch('\n');
}

void Terminal_Set_Color(enum vga_color color)
{
    terminal_color = color | COLOR_BLACK << 4;
}

void Terminal_Backspace(void)
{
    if (terminal_column > 0)
    {
        terminal_column--;
        Terminal_Putch(' ');
        terminal_column--;
    }
}

void Terminal_Newline(void)
{
    Terminal_Putch('\n');
}


// Clears the screen, and moves the text position to (0,0)
void Terminal_Clear(void)
{
    terminal_row = 0;
    terminal_column = 0;

	for ( uint32_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( uint32_t x = 0; x < VGA_WIDTH; x++ )
		{
			Terminal_Putch(' ');
		}
	}
	terminal_row = 0;
	terminal_column = 0;
}



void kprintf(const char *format, ...)
{
    char** arg = (char**) &format;
    char c;
    char buf[20];

    arg++;

    while((c = *format++) != 0)
    {
        if(c != '%')
        {
            Terminal_Putch(c);
        }
        else {
            char* p;
            c = *format++;
            switch(c)
            {
                case 'd':
                case 'u':
                case 'x':
                {
                    itoa(*((int*) arg++), buf, c);
                    p = buf;

                    goto string;
                    break;                    
                }

                case 's':
                {
                    p = *arg++;
                    if(!p) 
                    {
                        p = (char*)"(null)";

                    }

                    string:
                        Terminal_Print(p);
                        break;                      
                }

                case 0:
                {
                    return; 
                    break;                   
                }

                default:
                {
                    Terminal_Putch((char)*((int*) arg++));
                    break;                    
                }

            }
        }
    }
    Terminal_Update_Cursor();
}

void Terminal_Update_Cursor(void)
{
    uint32_t cursorLocation = (terminal_row * 80) + terminal_column;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (cursorLocation & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((cursorLocation >> 8) & 0xFF ));
}

