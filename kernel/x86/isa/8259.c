#include <x86/io.h>
#include <x86/video/vga.h>
#include <stdint.h>

#define PIC_MASTER          0x20
#define PIC_SLAVE           0xA0
#define PIC_MASTER_COMMAND  0x20 
#define PIC_MASTER_DATA     0x21 
#define PIC_SLAVE_COMMAND   0xA0
#define PIC_SLAVE_DATA      0xA1
#define PIC_CMD_ENDINTR     0x20
#define PIC_READ_IRR        0x0A
#define PIC_READ_ISR        0x0B
#define PIC_ICW1            0x11
#define PIC_ICW4_MASTER     0x01
#define PIC_ICW4_SLAVE      0x05
#define PIC_ACK_SPECIFIC    0x60
#define ICW1_ICW4	        0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE 	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	    0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	        0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	        0x10		/* Initialization - required! */ 
#define ICW4_8086	        0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	        0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	    0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	    0x0C		/* Buffered mode/master */
#define ICW4_SFNM	        0x10		/* Special fully nested (not) */


void Initialize_8259_PIC(uint32_t PIC_MASTER_BASE ,uint32_t PIC_SLAVE_BASE)
{
	uint8_t MASTER_MASK;
	uint8_t SLAVE_MASK;

	MASTER_MASK = inb(PIC_MASTER_DATA);
	SLAVE_MASK  = inb(PIC_SLAVE_DATA);

    outb(PIC_MASTER_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    outb(PIC_SLAVE, ICW1_INIT + ICW1_ICW4);
    io_wait();

    outb(PIC_MASTER_DATA, PIC_MASTER_BASE);
    io_wait();
    outb(PIC_SLAVE_DATA, PIC_SLAVE_BASE);
    io_wait();

    outb(PIC_MASTER_DATA, 0x04);
    io_wait();
    outb(PIC_SLAVE_DATA, 0x02);
    io_wait();

    outb(PIC_MASTER_DATA, ICW4_8086);
    io_wait();   
    outb(PIC_SLAVE_DATA, ICW4_8086);
    io_wait();
    outb(PIC_MASTER_DATA, MASTER_MASK);
    outb(PIC_SLAVE_DATA, SLAVE_MASK);

    kprintf("8259 PIC Successfully Initilialized\n");
}

void Enable_8259_PIC(uint8_t irq)
{
	uint8_t mask;
	if (irq < 8)
	{
		mask = inb(PIC_MASTER_DATA);
		mask = mask&~(1 << irq);
		outb(mask, PIC_MASTER_DATA);
	}
	else
	{
		irq -= 8;
		mask = inb(PIC_SLAVE_DATA);
		mask = mask&~(1 << irq);
		outb(mask, PIC_SLAVE_DATA);
		Enable_8259_PIC(2);
	}
}

void Disable_8259_PIC(uint8_t irq)
{
	uint8_t mask;
	if (irq < 8)
	{
		mask = inb(PIC_MASTER_DATA);
		mask = mask | (1 << irq);
		outb(mask, PIC_MASTER_DATA);
	}
	else
	{
		irq -= 8;
		mask = inb(PIC_SLAVE_DATA);
		mask = mask | (1 << irq);
		outb(mask, PIC_SLAVE_DATA);
	}
}


void Acknowledge_8259_PIC( uint8_t irq )
{	
	if(irq>=8) 
	{
		outb(PIC_ACK_SPECIFIC+(irq-8),PIC_SLAVE_COMMAND);
		outb(PIC_ACK_SPECIFIC+(2),PIC_MASTER_COMMAND);
	} 
	else 
	{
		outb(PIC_ACK_SPECIFIC+irq,PIC_MASTER_COMMAND);
	}
}


void PIC_EOI_Master(void)
{
	outb(PIC_MASTER, PIC_CMD_ENDINTR);
}

void PIC_EOI_Slave(void)
{
	outb(PIC_SLAVE, PIC_CMD_ENDINTR);
}

uint16_t PIC_Read_IRR(void)
{
	outb(PIC_MASTER_COMMAND, PIC_READ_IRR);
	outb(PIC_SLAVE_COMMAND, PIC_READ_IRR);
	return inb(PIC_MASTER_COMMAND) << 0 |
	       inb(PIC_SLAVE_COMMAND) << 8;
}

uint16_t PIC_Read_ISR(void)
{
	outb(PIC_MASTER_COMMAND, PIC_READ_ISR);
	outb(PIC_SLAVE_COMMAND, PIC_READ_ISR);
	return inb(PIC_MASTER_COMMAND) << 0 |
	       inb(PIC_SLAVE_COMMAND) << 8;
}
