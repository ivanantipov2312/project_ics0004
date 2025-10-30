#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_
#include <stdint.h>
#include <stdbool.h>

struct Timestamp {
	uint8_t hours;
	uint8_t minutes;

	uint8_t day;
	uint8_t month;
	uint16_t year;
};

bool timestamp_from_string(const char* date, struct Timestamp* result);
uint8_t timestamp_diff(struct Timestamp date1, struct Timestamp date2);
double timestamp_penalty(struct Timestamp current_date, struct Timestamp time_of_departure); 
void timestamp_print(struct Timestamp time); 

#endif // TIMESTAMP_H_
