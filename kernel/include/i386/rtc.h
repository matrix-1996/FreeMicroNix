#ifndef _I386_RTC_H
#define _I386_RTC_H

void rtc_initialize(void);
void isr_irq_rtc(/*struct regs* regs*/);

#endif