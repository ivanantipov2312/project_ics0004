#include "queue.h"
#include "timestamp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Add the item to the end
void queue_push(struct RecordQueue* q, const char* destination, const char* departure_datetime, const char* type_of_coach, float ticket_price, bool available, const char* passport_id) {
	if (!q->head) {
		q->head = q->tail = record_new(q->nextID, destination, departure_datetime, type_of_coach, ticket_price, available, passport_id);
		q->nextID++;
		return;
	}

	struct Record* tmp = record_new(q->nextID, destination, departure_datetime, type_of_coach, ticket_price, available, passport_id);
	q->nextID++;
	q->tail->next = tmp;
	q->tail = tmp;
}

// Delete the first item
void queue_pop(struct RecordQueue* q) {
	if (!q->head) {
		return;
	}

	struct Record* tmp = q->head;
	q->head = q->head->next;
	record_free(tmp);
	if (q->head == NULL) {
		q->tail = NULL;
	}
}

// Make queue empty
void queue_clear(struct RecordQueue* q) {
	while (q->head) {
		queue_pop(q);
	}
	q->nextID = 1;
}

bool queue_is_empty(struct RecordQueue q) {
	return q.head == NULL;
}

void queue_search(struct Record** match, struct RecordQueue query, const char* destination, const char* departure_datetime, const char* type_of_coach, const char* passport_id) {
	struct Record* tmp = query.head;

	if (passport_id != NULL) {
		while (tmp) {
			if(!strcmp(tmp->passport_id,passport_id)) {
				*match = tmp;
				break;
			} else{
				tmp = tmp->next;
			}
		}
	} else {
		struct Timestamp q_timestamp;
		timestamp_from_string(departure_datetime, &q_timestamp);

		while (tmp) {
			if (!strcmp(tmp->destination,destination) && !timestamp_diff(tmp->departure_timestamp,q_timestamp) && !strcmp(tmp->type_of_coach,type_of_coach)) {
				*match = tmp;
				break;
			} else {
				tmp = tmp->next;
			}
		}
	}
}

void queue_print(struct RecordQueue q, int options) {
	if (!q.head) {
		printf("No records!\n");
		return;
	}

	struct Record* tmp = q.head;
	while (tmp) {
		if (options == 0 || (options == 1 && tmp->available)) {
			record_print(tmp);
		}

		tmp = tmp->next;
	}
}
