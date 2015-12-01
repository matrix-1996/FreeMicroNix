#include <i386/global.h>


int64_t current_time = 0;
int current_weekday = 0;

#define leap_year(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)
int month_lengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, };

int64_t time_set(int64_t newtime) {
	current_time = newtime;
	return current_time;
}

int64_t time_get(void) {
	return current_time;
}

int time_get_weekday(void) {
	return current_weekday;
}

int64_t time_timestamp(datetime_t dt) {
	int days = 0;
	int64_t new_time = 0;
	
	if (dt.year >= 1970) {
		for (int i = 0; i < dt.month-1; i++) {
			if (i == 1 && leap_year(dt.year))
				days += 1;
			days += month_lengths[i];
		}
	
		while (dt.year > 1970) {
			dt.year--;
			days += (leap_year(dt.year) ? 366 : 365);
		}
		days += dt.day-1;
	
		new_time = 86400 * days;
		new_time += 3600 * dt.hour;
		new_time += 60 * dt.minute;
		new_time += dt.second;
	} else {
		for (int i = 11; i > dt.month-1; i--) {
			if (i == 1 && leap_year(dt.year))
				days += 1;
			days += month_lengths[i];
		}
		
		while (dt.year < 1969) {
			dt.year++;
			days += (leap_year(dt.year)? 366 : 365);
		}
		days += month_lengths[dt.month-1] - dt.day;
		
		new_time = 0 - 86400 * days;
		new_time -= ((23 - dt.hour) * 3600);
		new_time -= ((59 - dt.minute) * 60);
		new_time -= (60 - dt.second);
		// + ((59 - dt.minute) * 60) + (59 - dt.second));
	}
	
	return new_time;
}



uint64_t cpu_rdtsc() {
    uint64_t ret;
    asm volatile ("rdtsc" : "=A"(ret));
    return ret;
}
