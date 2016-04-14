#ifndef _X86_RTC_H
#define _X86_RTC_H

#include <x86/isa/interrupt.h>
#include <stdint.h>
#include <x86/io.h>
#include <stdbool.h>

#define CURRENT_YEAR				2016

typedef struct rtc_time
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint32_t year;
} rtc_time_t;

void RTC_Update(void);
rtc_time_t* RTC_Get_Current_Time(void);
void RTC_Handler(interrupt_context_t *int_ctx);
void Initialize_RTC(void);
uint32_t Get_RTC_Ticks(void);

#endif