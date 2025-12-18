#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_write(struct RecordQueue q, const char* filename) {
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open or create the file!\n");
		return;
	}

	// Paste the headers
	fprintf(fp, "ID,Destination,Departure,Type,Price,Available,Passport\n");

	// Write one by one
	struct Record* r = q.head;
	while (r) {
		char* departure = timestamp_to_string(r->departure_timestamp);

		fprintf(fp, "%u,%s,%s,%s,%.2f,%s,%s\n",
			r->id,
			r->destination,
			departure,
			r->type_of_coach,
			r->ticket_price,
			(r->available ? "Yes" : "No"),
			r->passport_id
		);

		free(departure);

		r = r->next;
	}

	fclose(fp);
}

void file_read(struct RecordQueue* q, const char* filename) {
	if (q == NULL) {
		fprintf(stderr, "Queue is NULL!\n");
		return;
	}

	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open the file!\n");
		return;
	}

	// Skip the first line (table headers)
	char line[512];
	fgets(line, sizeof(line), fp);

	while (fgets(line, sizeof(line), fp) != NULL) {
		// Extract all fields, except for the firt one (ID is dynamic)
		strtok(line, ",");
		char* destination = strtok(NULL, ",");
		char* departure_time = strtok(NULL, ",");
		char* type_of_coach = strtok(NULL, ",");
		char* price_str = strtok(NULL, ",");
		float price = strtof(price_str, NULL);
		char* available_str = strtok(NULL, ",");
		bool available = strcmp(available_str, "Yes") == 0;
		char* passport_id = strtok(NULL, "\n");

		// Push the new entry to our queue
		queue_push(q, destination, departure_time, type_of_coach, price, available, passport_id);
	}

	fclose(fp);
}
