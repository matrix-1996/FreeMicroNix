#include <x86/rtc.h>
#include <x86/video/vga.h>


uint32_t rtc_ticks = 0;
rtc_time_t* rtc_current;
rtc_time_t*	rtc_last;
bool rtc_update_in_progress = false;


static bool RTC_read_update_in_progress(void)
{
	return CMOS_Read(0x0A) >> 7;	// 8th bit indictates if update in progress
}

static void RTC_wait_update_in_progress(void)
{
	do
	{
		rtc_update_in_progress = RTC_read_update_in_progress();
	}while(rtc_update_in_progress == true);
}

static int RTC_bcd_binary_conv(int toconv)
{
	return (toconv & 0x0F) + ((toconv / 16) * 10);
}

void RTC_Update(void)
{
	bool rtc_bcd = (CMOS_Read(0x0B) == 2) ? (true) : (false);
	bool rtc_24hr = (CMOS_Read(0x0B) == 4) ? (true) : (false);
	
	uint8_t century_register = 0x32;	// 21st century skitzoid man!!!
	//uint8_t century_register = 0;

	do
	{
		rtc_last = rtc_current;

		RTC_wait_update_in_progress();

		rtc_current->seconds = CMOS_Read(0x00);
		rtc_current->minutes = CMOS_Read(0x02);
		rtc_current->hours = CMOS_Read(0x04);
		rtc_current->weekday = CMOS_Read(0x06);
		rtc_current->day = CMOS_Read(0x07);
		rtc_current->month = CMOS_Read(0x08);
		rtc_current->year = CMOS_Read(0x09);

		if (century_register != 0)
		{
			rtc_current->century = CMOS_Read(century_register);
		}
	} while((rtc_current->seconds != rtc_last->seconds)
        || (rtc_current->minutes != rtc_last->minutes)
        || (rtc_current->hours != rtc_last->hours)
        || (rtc_current->weekday != rtc_last->weekday)
        || (rtc_current->day != rtc_last->day)
        || (rtc_current->month != rtc_last->month)
        || (rtc_current->year != rtc_last->year)
        || (rtc_current->century != rtc_last->century));

	// Binary Conversions
	if (rtc_bcd)
	{
        rtc_current->seconds    = RTC_bcd_binary_conv(rtc_current->seconds);
        rtc_current->minutes    = RTC_bcd_binary_conv(rtc_current->minutes);
        rtc_current->hours      = RTC_bcd_binary_conv(rtc_current->hours);
        rtc_current->day        = RTC_bcd_binary_conv(rtc_current->day);
        rtc_current->month      = RTC_bcd_binary_conv(rtc_current->month);
        rtc_current->year       = RTC_bcd_binary_conv(rtc_current->year);

        if (century_register != 0)
        {
        	rtc_current->century = RTC_bcd_binary_conv(rtc_current->century);
        }
    }

    else if (rtc_24hr)
    {
    	if (rtc_current->hours & 0x80)
    	{
    		rtc_current->hours = ((rtc_current->hours & 0x7F) + 12) % 24;
    		rtc_current->am = false;
    	}

    	else
    	{
    		rtc_current->am = true;
    	}

    }

    if (century_register != 0)
    {
    	uint16_t temp = rtc_current-> year + rtc_current->century * 100;
    	if (temp < RTC_SANITY_CHECK_YEAR_LOW)
    	{
    		rtc_current->year += 2000;

    	}
    	else if (temp > RTC_SANITY_CHECK_YEAR_HIGH)
    	{
    		rtc_current->year += 2000;
    	}
    	else
    	{
    		rtc_current->year = temp;
    	}
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
	Install_Interrupt_Handler(8, RTC_Handler);	// Install the handler
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

