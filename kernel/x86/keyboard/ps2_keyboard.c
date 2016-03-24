#include <x86/isa/interrupt.h>
#include <x86/keyboard/ps2_keyboard.h>
#include <x86/io.h>
#include <x86/ascii.h>
#include <x86/video/vga.h>
#include <kernel/reboot.h>
#include <datastruct/list.h>

#define KEY_INVALID 127

#define SPECIAL_SHIFT 	  1
#define SPECIAL_ALT 	  2
#define SPECIAL_CTRL	  3
#define SPECIAL_SHIFTLOCK 4

#define KEYBOARD_PORT_A 0x60
#define KEYBOARD_PORT_B 0x61
#define KEYBOARD_ACK 0x80

#define BUFFER_SIZE 256

typedef struct keymap
{
	char normal;
	char shifted;
	char ctrled;
	char special;
} keymap_t;

static keymap_t keymap[] = 
{
	#include <keymap.us.c>
};

static char buffer[BUFFER_SIZE];
static int buffer_read = 0;
static int buffer_write = 0;

static list_t queue = {0,0};

static int keyboard_scan()
{

	int code, ack;
	code = inb(KEYBOARD_PORT_A);
	ack = inb(KEYBOARD_PORT_B);
	ack |= KEYBOARD_ACK;
	outb(KEYBOARD_PORT_B, ack);
	outb(KEYBOARD_PORT_B,ack);
	return code;
}

static int shift_mode = 0;
static int alt_mode = 0;
static int ctrl_mode = 0;
static int capslock_mode = 0;

static char keyboard_map( int code )
{
	int direction;

	if (code & 0x80)
	{
		direction = 0;
		code = code & 0x7f;
	}

	else 
	{
		direction = 1;
	}

	if (keymap[code].special == SPECIAL_SHIFT)
	{
		shift_mode = direction;
		return KEY_INVALID;
	}

	else if (keymap[code].special == SPECIAL_ALT)
	{
		alt_mode = direction;
		return KEY_INVALID;
	}

	else if (keymap[code].special == SPECIAL_SHIFTLOCK)
	{
		if (direction == 0) capslock_mode = !capslock_mode;
		return KEY_INVALID;
	}

	else if (direction)
	{
		if (ctrl_mode && alt_mode && keymap[code].normal == ASCII_DEL)
		{
			kprintf("CTRL+ALT+DEL Pressed\n");
			reboot();
			return KEY_INVALID;
		}

		else if (capslock_mode)
		{
			if (shift_mode)
			{
				return keymap[code].normal;
			}

			else
			{
				return keymap[code].shifted;
			}
		}

		else if (shift_mode)
		{
			return keymap[code].shifted;
		}
		else if (ctrl_mode)
		{
			return keymap[code].ctrled;
		}
		else 
		{
			return keymap[code].normal;
		}
	}

	else 
		{
			return KEY_INVALID;
		}
}


void keyboard_handler(interrupt_context_t *int_ctx)
{
	char c;
	c = keyboard_map(keyboard_scan());
	
	if (c == KEY_INVALID)
	{
		return;
	}

	if ((buffer_write + 1) == (buffer_read % BUFFER_SIZE))
	{
		return;
	}

	buffer[buffer_write] = c;

	buffer_write = (buffer_write + 1) % BUFFER_SIZE;


}


void Initialize_PS2_Keyboard()
{
	if (keyboard_scan()) // If keyboard scan is succesful
	{
			Install_Interrupt_Handler(1, keyboard_handler);
			kprintf("PS/2 Keyboard Successfully Initialized\n");
	}
	else
	{
		kprintf("PS/2 Keyboard Failed to Initialize\n");
	}

}

