#include "record.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Record* record_new(uint32_t id, const char* destination, const char* departure_datetime,
		const char* arrival_datetime, const char* type_of_coach, double ticket_price, const char* purchase_datetime, bool available) {
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

	len = strlen(departure_datetime);
	rec->departure_datetime = malloc(sizeof(char) * (len + 1));
	snprintf(rec->departure_datetime, len + 1, "%s", departure_datetime);
	rec->departure_datetime[len] = '\0';

	len = strlen(arrival_datetime);
	rec->arrival_datetime = malloc(sizeof(char) * (len + 1));
	snprintf(rec->arrival_datetime, len + 1, "%s", arrival_datetime);
	rec->arrival_datetime[len] = '\0';

	const char* type_ptr = type_of_coach ? type_of_coach : "Unspecified";
	len = strlen(type_ptr);
	rec->type_of_coach = malloc(sizeof(char) * (len + 1));
	snprintf(rec->type_of_coach, len + 1, "%s", type_ptr);
	rec->type_of_coach[len] = '\0';

	len = strlen(purchase_datetime);
	rec->purchase_datetime = malloc(sizeof(char) * (len + 1));
	snprintf(rec->purchase_datetime, len + 1, "%s", purchase_datetime);
	rec->purchase_datetime[len] = '\0';

	rec->next = NULL;

	return rec;
}

// Free all the memory behind the record
void record_free(struct Record* rec) {
	if (rec) {
		rec->id = 0;
		rec->available = false;
		rec->ticket_price = 0;

		if (rec->destination) {
			free(rec->destination);
			rec->destination = NULL;
		}

		if (rec->departure_datetime) {
			free(rec->departure_datetime);
			rec->departure_datetime = NULL;
		}

		if (rec->arrival_datetime) {
			free(rec->arrival_datetime);
			rec->arrival_datetime = NULL;
		}

		if (rec->type_of_coach) {
			free(rec->type_of_coach);
			rec->type_of_coach = NULL;
		}

		if (rec->purchase_datetime) {
			free(rec->purchase_datetime);
			rec->purchase_datetime = NULL;
		}

		free(rec);
		rec = NULL;
	}
}

void record_print(struct Record* rec) {
	printf("ID: %u, Destination: %s, Departing: %s, Arriving: %s, Type of Coach: %s, Ticket Price: %.2lf, Purchase Time: %s, Available: %s\n",
			rec->id, rec->destination, rec->departure_datetime, rec->arrival_datetime, rec->type_of_coach, rec->ticket_price, rec->purchase_datetime, rec->available ? "Yes": "No");
}
