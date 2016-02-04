#include <io.h>
#include <video/vga.h>

void kmain()
{
	init_terminal();
	terminal_writestring("FreeMicroNix Version 0.01 Reborn\n");
}