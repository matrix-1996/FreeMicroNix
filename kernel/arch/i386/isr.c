#include <kernel/common.h>
#include <kernel/isr.h>
#include <kernel/tty.h>

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
   printf("recieved interrupt: ");
   terminal_write_decimal(regs.int_no);
   terminal_putchar('\n');
} 