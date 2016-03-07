void init_pic()
{
    outb(MASTER_PIC_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();
    outb(SLAVE_PIC_COMMAND, ICW1_INIT + ICW1_ICW4);
    io_wait();

    outb(MASTER_PIC_DATA, 0x20);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x70);
    io_wait();

    outb(MASTER_PIC_DATA, 0x04);
    io_wait();
    outb(SLAVE_PIC_DATA, 0x02);
    io_wait();

    outb(MASTER_PIC_DATA, ICW4_8086);
    io_wait();   
    outb(SLAVE_PIC_DATA, ICW4_8086);
    io_wait();
    outb(MASTER_PIC_DATA, 0x0);
    outb(SLAVE_PIC_DATA, 0x0);
}
