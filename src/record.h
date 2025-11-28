#ifndef RECORD_H_
#define RECORD_H_
#include <stdint.h>
#include <stdbool.h>
#include "timestamp.h"

struct Record {
	uint32_t id;
	char* destination;
	struct Timestamp departure_timestamp;
	struct Timestamp arrival_timestamp;
	char* type_of_coach;
	float ticket_price;
	struct Timestamp purchase_timestamp;
	bool available;
	struct Record* next; // For queue
};

struct Record* record_new(uint32_t id, const char* destination, const char* departure_datetime,
		const char* arrival_datetime, const char* type_of_coach, float ticket_price, const char* purchase_datetime, bool available);
void record_free(struct Record* rec);
void record_print(struct Record* rec);

#endif // RECORD_H_
