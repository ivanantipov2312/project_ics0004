#include "timestamp.h"
#include "utils.h"
#include <stdio.h>

uint8_t timestamp_diff(struct Timestamp date1, struct Timestamp date2) {
	int days1 = date1.day + (date1.month - 1) * get_days_from_month(date1.month, date1.year) + date1.year * is_leap_year(date1.year) ? 366 : 365;
	int days2 = date2.day * (date2.month - 1) * get_days_from_month(date2.month, date2.year) + date2.year * is_leap_year(date2.year) ? 366 : 365;
	return days2 - days1;
}

bool timestamp_from_string(const char* date, struct Timestamp* result) {
	// Format check
	if (sscanf(date, "%hhu:%hhu-%hhu/%hhu/%hu", &result->hours, &result->minutes, &result->day, &result->month, &result->year) != 5) {
		printf("Error: Invalid date. Format is hh:mm-DD/MM/YYYY\n");
		return false;
	}

	// Bounds checking
	if (result->day > 31 || result->month > 12 || result->minutes > 60 || result->hours > 24) {
		printf("Error: Out of bounds value in date.\n");
		return false;
	}

	return true;
}

void timestamp_print(struct Timestamp time) {
	printf("%u:%u-%u/%u/%u", time.hours, time.minutes, time.day, time.month, time.year);
}

double timestamp_penalty(struct Timestamp current_date, struct Timestamp time_of_departure) {
	uint8_t days_diff = timestamp_diff(current_date, time_of_departure);
	if (days_diff > 30) {
		return 0.01;
	} else if (days_diff > 15) {
		return 0.05;
	} else if (days_diff > 3) {
		return 0.10;
	}

	return 0.30;
}


