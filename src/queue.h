#ifndef QUEUE_H_
#define QUEUE_H_
#include "record.h"
#include <stdint.h>

struct RecordQueue {
	struct Record* head;
	struct Record* tail;
	uint32_t nextID;
};

// Add the item to the end
void queue_push(struct RecordQueue* q, const char* destination, const char* departure_datetime,
		const char* type_of_coach, float ticket_price, bool available, const char* passport_id);

// Delete the first item
void queue_pop(struct RecordQueue* q);

// Make queue empty
void queue_clear(struct RecordQueue* q);

// Util functions
bool queue_is_empty(struct RecordQueue q);
void queue_search(struct Record** match, struct RecordQueue query, const char* destination, const char* departure_datetime, const char* type_of_coach, const char* passport_id);
void queue_print(struct RecordQueue q, int options);

#endif // QUEUE_H_
