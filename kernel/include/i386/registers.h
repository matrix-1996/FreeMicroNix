#ifndef _I386_REGISTERS_H
#define _I386_REGISTERS_H


const size_t FLAGS_CARRY        = 1 << 0; // 0x000001
const size_t FLAGS_RESERVED1    = 1 << 1; // 0x000002, read as one
const size_t FLAGS_PARITY       = 1 << 2; // 0x000004
const size_t FLAGS_RESERVED2    = 1 << 3; // 0x000008
const size_t FLAGS_AUX          = 1 << 4; // 0x000010
const size_t FLAGS_RESERVED3    = 1 << 5; // 0x000020
const size_t FLAGS_ZERO         = 1 << 6; // 0x000040
const size_t FLAGS_SIGN         = 1 << 7; // 0x000080
const size_t FLAGS_TRAP         = 1 << 8; // 0x000100
const size_t FLAGS_INTERRUPT    = 1 << 9; // 0x000200
const size_t FLAGS_DIRECTION    = 1 << 10; // 0x000400
const size_t FLAGS_OVERFLOW     = 1 << 11; // 0x000800
const size_t FLAGS_IOPRIVLEVEL  = 1 << 12 | 1 << 13;
const size_t FLAGS_NESTEDTASK   = 1 << 14; // 0x004000
const size_t FLAGS_RESERVED4    = 1 << 15; // 0x008000
const size_t FLAGS_RESUME       = 1 << 16; // 0x010000
const size_t FLAGS_VIRTUAL8086  = 1 << 17; // 0x020000
const size_t FLAGS_ALIGNCHECK   = 1 << 18; // 0x040000
const size_t FLAGS_VIRTINTR     = 1 << 19; // 0x080000
const size_t FLAGS_VIRTINTRPEND = 1 << 20; // 0x100000
const size_t FLAGS_ID           = 1 << 21; // 0x200000

struct interrupt_context
{
	unsigned int signal_pending;
	unsigned int kerrno;
	unsigned int cr2;
	unsigned int ds;
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int not_esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned int int_no;
	unsigned int err_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
	unsigned int esp;
	unsigned int ss;
};

#endif