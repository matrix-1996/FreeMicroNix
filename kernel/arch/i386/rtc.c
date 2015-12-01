#include <i386/global.h>

#define CMOS_PORT_INDEX 0x70
#define CMOS_PORT_DATA 0x71

#define CMOS_NMI_DISABLE 0x80

#define CMOS_REGISTER_SECOND	0x00
#define CMOS_REGISTER_MINUTE	0x02
#define CMOS_REGISTER_HOUR		0x04
#define CMOS_REGISTER_WEEKDAY	0x06
#define CMOS_REGISTER_DAY		0x07
#define CMOS_REGISTER_MONTH		0x08
#define CMOS_REGISTER_YEAR		0x09
#define CMOS_REGISTER_CENTURY	0x32

#define CMOS_REGISTER_STATUS_A	0x0A
#define CMOS_REGISTER_STATUS_B	0x0B
#define CMOS_REGISTER_STATUS_C	0x0C

#define CMOS_INTERRUPT_ANY		0x80
#define CMOS_INTERRUPT_PERIODIC	0x40
#define CMOS_INTERRUPT_ALARM	0x20
#define CMOS_INTERRUPT_UPDATE	0x10

extern int current_weekday;

bool rtc_initialized = false;
bool rtc_bcd = true;

unsigned long long last_tsc = 0;
unsigned long long  recent_tsc = 0;

// Change a BCD byte to a decimal integer.
int bcdb_to_dec(unsigned char bcd) {
	return ((bcd >> 4) * 10 + (bcd & 0x0F));
}

// Resynchronize our timekeeping with the RTC's.
void rtc_synchronize(void) {
	int century;
	long long new_time = 0;
	datetime_t dt;
	
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_SECOND);
	dt.second = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_MINUTE);
	dt.minute = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_HOUR);
	dt.hour = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_DAY);
	dt.day = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_MONTH);
	dt.month = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_YEAR);
	dt.year = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_CENTURY);
	century = inb(CMOS_PORT_DATA);
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_WEEKDAY);
	dt.weekday = inb(CMOS_PORT_DATA);
	
	// Convert from BCD to regular binary decimal format.
	if (rtc_bcd) {
		dt.second = bcdb_to_dec(dt.second);
		dt.minute = bcdb_to_dec(dt.minute);
		dt.hour = bcdb_to_dec(dt.hour);
		dt.day = bcdb_to_dec(dt.day);
		dt.month = bcdb_to_dec(dt.month);
		dt.year = bcdb_to_dec(dt.year);
		century = bcdb_to_dec(century);
	}
	
	// Set the current day of the week.
	current_weekday = dt.weekday;
	
	// Make the year field the four digit year.
	dt.year = dt.year + century * 100;
	
	// Convert our datetime structure to a 64-bit timestamp and save it.
	new_time = time_timestamp(dt);
	time_set(new_time);
}

// This function is a bit of a misnomer. We don't actually reinitialize the
// RTC chip itself here. That would be bad, since we'd lose the system's
// time settings. Rather we initialize what we want the RTC chip to fire
// interrupts for and start keeping track of time.
void rtc_initialize(void) {
	unsigned char status = 0;
	
	asm volatile ("cli");
	
	outb(CMOS_PORT_INDEX, CMOS_NMI_DISABLE | CMOS_REGISTER_STATUS_B);
	status = inb(CMOS_PORT_DATA);
	status = status | CMOS_INTERRUPT_UPDATE;
	outb(CMOS_PORT_INDEX, CMOS_NMI_DISABLE | CMOS_REGISTER_STATUS_B);
	outb(CMOS_PORT_DATA, status);
	
	outb(CMOS_PORT_INDEX, CMOS_REGISTER_STATUS_C);
	inb(CMOS_PORT_DATA);
	asm volatile ("sti");

	if (status & 0x04)
		rtc_bcd = false;				// I don't think I've ever seen a system set to binary mode, but if it does, handle it.
	else
		rtc_bcd = true;
	
	rtc_initialized = true;
	rtc_synchronize();
	//kprintf("[RTC] New timestamp: %llu\n", time_get());
	recent_tsc = cpu_rdtsc();
}

void isr_irq_rtc(/*struct regs* regs*/) {
	unsigned char status = 0;
	if (!rtc_initialized) {
		outb(0xA0, 0x20);
		outb(0x20, 0x20);
		return;
	}

	outb(CMOS_PORT_INDEX, CMOS_REGISTER_STATUS_C);
	status = inb(CMOS_PORT_DATA);
	
	if (status & CMOS_INTERRUPT_UPDATE) {
		time_set(time_get() + 1);
		if (time_get() % 10 == 0) {
			rtc_synchronize();
			//kprintf("Time synced with RTC. ");
		}
		
		//kprintf("Current timestamp: %lld\n", time_get());
		
		last_tsc = recent_tsc;
		recent_tsc = cpu_rdtsc();
		//kprintf("Possible clock frequency: %llu Hz\n", recent_tsc-last_tsc);
	}
		
	outb(0xA0, 0x20);
	outb(0x20, 0x20);
}
