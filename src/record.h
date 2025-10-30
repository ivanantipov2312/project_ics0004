#ifndef RECORD_H_
#define RECORD_H_
#include <stdint.h>
#include <stdbool.h>

struct Record {
	uint32_t id;
	char* destination;
	char* departure_datetime;
	char* arrival_datetime;
	char* type_of_coach;
	double ticket_price;
	char* purchase_datetime;
	bool available;
	struct Record* next; // For queue
};

struct Record* record_new(uint32_t id, const char* destination, const char* departure_datetime,
		const char* arrival_datetime, const char* type_of_coach, double ticket_price, const char* purchase_datetime, bool available);
void record_free(struct Record* rec);
void record_print(struct Record* rec);

#endif // RECORD_H_
