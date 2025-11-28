#include "record.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Record* record_new(uint32_t id, const char* destination, const char* departure_timestamp,
		const char* arrival_timestamp, const char* type_of_coach, float ticket_price, const char* purchase_timestamp, bool available) {
	struct Record* rec = malloc(sizeof(*rec));
	rec->id = id;
	rec->available = available;
	rec->ticket_price = ticket_price;

	// Write data to the structure
	// len + 1 because snprintf adds null-termination
	size_t len = strlen(destination);
	rec->destination = malloc(sizeof(char) * (len + 1));
	snprintf(rec->destination, len + 1, "%s", destination);
	rec->destination[len] = '\0';

	timestamp_from_string(departure_timestamp, &rec->departure_timestamp);
	timestamp_from_string(arrival_timestamp, &rec->arrival_timestamp);

	const char* type_ptr = type_of_coach ? type_of_coach : "Unspecified";
	len = strlen(type_ptr);
	rec->type_of_coach = malloc(sizeof(char) * (len + 1));
	snprintf(rec->type_of_coach, len + 1, "%s", type_ptr);
	rec->type_of_coach[len] = '\0';

	timestamp_from_string(purchase_timestamp, &rec->purchase_timestamp);

	rec->next = NULL;

	return rec;
}

// Free all the memory behind the record
void record_free(struct Record* rec) {
	if (rec) {
		rec->id = 0;
		rec->available = false;
		rec->ticket_price = 0.0f;

		if (rec->destination) {
			free(rec->destination);
			rec->destination = NULL;
		}

		if (rec->type_of_coach) {
			free(rec->type_of_coach);
			rec->type_of_coach = NULL;
		}

		free(rec);
		rec = NULL;
	}
}

void record_print(struct Record* rec) {
	printf("%u,%s,", rec->id, rec->destination);
	timestamp_print(rec->departure_timestamp);
	printf(",");
	timestamp_print(rec->arrival_timestamp);
	printf(",%s,%.2f,", rec->type_of_coach, rec->ticket_price);
	timestamp_print(rec->purchase_timestamp);
	printf(",%s\n", (rec->available ? "Yes" : "No" ));
}
