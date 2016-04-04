#ifndef _X86_RTC_H
#define _X86_RTC_H

#include <x86/cmos.h>
#include <x86/isa/interrupt.h>
#include <stdbool.h>

#define RTC_SANITY_CHECK_YEAR_LOW	2016
#define RTC_SANITY_CHECK_YEAR_HIGH	2099


typedef struct rtc_time
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t weekday;
	uint8_t day;
	uint8_t month;
	uint8_t century;
	uint32_t year;
	bool am;
}rtc_time_t;

void RTC_Update(void);
rtc_time_t* RTC_Get_Current_Time(void);
void RTC_Handler(interrupt_context_t *int_ctx);
void Initialize_RTC(void);
uint32_t Get_RTC_Ticks(void);

#endif