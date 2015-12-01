#ifndef _I386_TIMER_H
#define _I386_TIMER_H

void timer_initialize(int frequency, bool reset_ticks);
void isr_irq_timer(/*struct regs* regs*/);
#endif