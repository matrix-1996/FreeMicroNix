/*
Copyright (C) <2016>  William Taylor Jones <freemicronix@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <x86/rtc.h>
#include <x86/video/vga.h>

uint32_t rtc_ticks = 0;
rtc_time_t* rtc_current;
bool rtc_update_in_progress = false;

enum {
    cmos_address = 0x70,
    cmos_data = 0x71
};

int RTC_Get_Update_In_Progress_Flag(void)
{
    outb(cmos_address, 0x0A);
    return (inb(cmos_data) & 0x80);
}

uint8_t RTC_Get_Register(int reg)
{
    outb(cmos_address, reg);
    return inb(cmos_data);
}



void RTC_Update(void)
{

    uint8_t century;
    uint8_t last_second;
    uint8_t last_minute;
    uint8_t last_hour;
    uint8_t last_day;
    uint8_t last_month;
    uint8_t last_year;
    uint8_t last_century;
    uint8_t registerB;


    while (RTC_Get_Update_In_Progress_Flag());
    
    rtc_current->second = RTC_Get_Register(0x00);
    rtc_current->minute = RTC_Get_Register(0x02);
    rtc_current->hour = RTC_Get_Register(0x04);
    rtc_current->day = RTC_Get_Register(0x07);
    rtc_current->month = RTC_Get_Register(0x08);
    rtc_current->year = RTC_Get_Register(0x09);

    int century_register = 0x00; 
    //int century_register = 0x32;

    if(century_register != 0)
    {
        century = RTC_Get_Register(century_register);
    }

	do
	{
        last_second = rtc_current->second;
        last_minute = rtc_current->minute;
        last_hour = rtc_current->hour;
        last_month = rtc_current->month;
        last_year = rtc_current->year;
        last_century = century;

        while(RTC_Get_Update_In_Progress_Flag());

        rtc_current->second = RTC_Get_Register(0x00);
        rtc_current->minute = RTC_Get_Register(0x02);
        rtc_current->hour = RTC_Get_Register(0x04);
        rtc_current->day = RTC_Get_Register(0x07);
        rtc_current->month =  RTC_Get_Register(0x08);
        rtc_current->year = RTC_Get_Register(0x09);

		if (century_register != 0)
		{
			century = RTC_Get_Register(century_register);
		}
	} while(    (last_second != rtc_current->second) || (last_minute != rtc_current->minute) || (last_hour != rtc_current->hour) ||
                (last_day != rtc_current->day) || (last_month != rtc_current->month) || (last_year != rtc_current->year ) || 
                (last_century != century) );
    registerB = RTC_Get_Register(0x0B);

    if (!(registerB & 0x04))
    {
        rtc_current->second = ( rtc_current->second & 0x0F ) + ((rtc_current->second / 16) * 10);
        rtc_current->minute = ( rtc_current->minute & 0x0F ) + ((rtc_current->minute / 16) * 10);
        rtc_current->hour = ( (rtc_current->hour & 0x0F) + (((rtc_current->hour & 0x70) / 16 ) * 10 )  | (rtc_current->hour & 0x80));
        rtc_current->day = ((rtc_current->day & 0x0F ) + ((rtc_current->day & 0x70) / 16) * 10);
        rtc_current->month = (rtc_current->month & 0x0F) + ((rtc_current->month / 16) * 10);
        rtc_current->year = (rtc_current->year & 0x0F) + ((rtc_current->year / 16) * 10);
        if (century_register != 0)
        {
            century = (century & 0x0F) + ((century / 16) * 10);            
        }
    }

    if (!(registerB & 0x02) && ( rtc_current->hour & 0x80))
    {
        rtc_current->hour = ((rtc_current->hour & 0x7F) + 12) % 24;
    }

    if (!(registerB & 0x02) && (rtc_current->hour & 0x80))
    {
        rtc_current->hour= ((rtc_current->hour & 0x7F) + 12) % 24;
    }

    if (century_register != 0)
    {
        rtc_current->year += century * 100;
    }
    else
    {
        rtc_current += (CURRENT_YEAR / 100) * 100;
        if (rtc_current->year < CURRENT_YEAR) rtc_current->year += 100;
    }
}

rtc_time_t* RTC_Get_Current_Time(void)
{
	return rtc_current;
}



void RTC_Handler(interrupt_context_t *int_ctx) 
{
	rtc_ticks++;
	outb(0x70, 0x0C);	// select register C
	inb(0x71);		//throw away contents
}

void Initialize_RTC(void)
{
	Install_Interrupt_Handler(IRQ8, RTC_Handler);	// Install the handler
	Disable_Interrupts();
	outb(0x70, 0x8B);		// Select register B and disable NMI
	char prev = inb(0x71);	// Read the current value of register B
	outb(0x70, 0x8B);
	outb(0x71, prev | 0x40);// Write the prev value, turning on bit 6 or register B
	Enable_Interrupts();
	kprintf("Real Time Clock Initialized\n");
}

uint32_t Get_RTC_Ticks(void)
{
	return rtc_ticks;
}

