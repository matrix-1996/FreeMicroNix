#ifndef _I386_INTERRUPT_H
#define _I386_INTERRUPT_H

struct interrupt_context
{
	uint32_t cr2;
	uint32_t gs;
	uint32_t fs;
	uint32_t ds;
	uint32_t es;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t int_no;
	uint32_t err_code;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp; /* If (cs & 0x3) != 0 */
	uint32_t ss;  /* If (cs & 0x3) != 0 */
};



void isr_handler(struct interrupt_context* int_ctx);

void irq_handler(struct interrupt_context* int_ctx);

void interrupt_handler(struct interrupt_context* int_ctx);

#endif