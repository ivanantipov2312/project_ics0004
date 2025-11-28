#include "queue.h"
#include <stdio.h>

// Add the item to the end
void queue_push(struct RecordQueue* q, const char* destination, const char* departure_datetime, const char* arrival_datetime, const char* type_of_coach, float ticket_price, const char* purchase_datetime, bool available) {
	if (!q->head) {
		q->head = q->tail = record_new(q->nextID, destination, departure_datetime, arrival_datetime, type_of_coach, ticket_price, purchase_datetime, available);
		q->nextID++;
		return;
	}

	struct Record* tmp = record_new(q->nextID, destination, departure_datetime, arrival_datetime, type_of_coach, ticket_price, purchase_datetime, available);
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

void queue_print(struct RecordQueue q) {
	if (!q.head) {
		printf("No records!\n");
		return;
	}

	struct Record* tmp = q.head;
	while (tmp) {
		record_print(tmp);
		tmp = tmp->next;
	}
}


