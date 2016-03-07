void panic(void)
{
	asm volatile ("cli");
	asm volatile ("hlt");
}