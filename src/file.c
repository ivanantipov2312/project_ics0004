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
	fprintf(fp, "ID,Destination,Departure,Arrival,Type,Price,Purchased,Available\n");

	// Write one by one
	struct Record* r = q.head;
	while (r) {
		char* departure = timestamp_to_string(r->departure_timestamp);
		char* arrival = timestamp_to_string(r->arrival_timestamp);
		char* purchase = timestamp_to_string(r->purchase_timestamp);

		fprintf(fp, "%u,%s,%s,%s,%s,%.2f,%s,%s\n",
			r->id,
			r->destination,
			departure,
			arrival,
			r->type_of_coach,
			r->ticket_price,
			purchase,
			(r->available ? "Yes" : "No")
		);

		free(departure);
		free(arrival);
		free(purchase);

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

	ssize_t read;
	char* line = NULL;
	size_t len = 0;

	// Skip the first line (table headers)
	read = getline(&line, &len, fp);

	while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", line);

		// Extract all fields, except for the firt one (ID is dynamic)
		strtok(line, ",");
		char* destination = strtok(NULL, ",");
		char* departure_time = strtok(NULL, ",");
		char* arrival_time = strtok(NULL, ",");
		char* type_of_coach = strtok(NULL, ",");
		char* price_str = strtok(NULL, ",");
		float price = atof(price_str);
		char* purchase_time = strtok(NULL, ",");
		char* available_str = strtok(NULL, ",");
		bool available = strcmp(available_str, "Yes") == 0;

		// Push the new entry to our queue
		queue_push(q, destination, departure_time, arrival_time, type_of_coach, price, purchase_time, available);
	}

	fclose(fp);
}
