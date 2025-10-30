#include <stdint.h>
#include <stdio.h>
// use true to distinguish true value from integer literal 1
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Timestamp {
	uint8_t hours;
	uint8_t minutes;

	uint8_t day;
	uint8_t month;
	uint16_t year;
};

bool is_leap_year(uint16_t year) {
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

uint8_t month_days(uint8_t month, int year) {
	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31;
	case 4: case 6: case 9: case 11:
		return 30;
	case 2: // Feb
		if (is_leap_year(year)) {
			return 29;
		} else {
			return 28;
		}
	default:
		return 0;
	}
}

uint8_t date_diff(struct Timestamp date1, struct Timestamp date2) {
	int days1 = date1.day + (date1.month - 1) * month_days(date1.month, date1.year) + date1.year * is_leap_year(date1.year) ? 366 : 365;
	int days2 = date2.day * (date2.month - 1) * month_days(date2.month, date2.year) + date2.year * is_leap_year(date2.year) ? 366 : 365;
	return days2 - days1;
}

bool timestamp_from_string(const char* date, struct Timestamp* result) {
	// Format check
	if (sscanf(date, "%hhu:%hhu-%hhu/%hhu/%hu", &result->hours, &result->minutes, &result->day, &result->month, &result->year) != 5) {
		printf("Error: Invalid date. Format is hh:mm-DD/MM/YYYY\n");
		return false;
	}

	// Bounds checking
	if (result->day > 31 || result->month > 12 || result->minutes > 60 || result->hours > 24) {
		printf("Error: Out of bounds value in date.\n");
		return false;
	}

	return true;
}

void timestamp_print(struct Timestamp time) {
	printf("%u:%u-%u/%u/%u", time.hours, time.minutes, time.day, time.month, time.year);
}

double timestamp_penalty(struct Timestamp current_date, struct Timestamp time_of_departure) {
	uint8_t days_diff = date_diff(current_date, time_of_departure);
	if (days_diff > 30) {
		return 0.01;
	} else if (days_diff > 15) {
		return 0.05;
	} else if (days_diff > 3) {
		return 0.10;
	}

	return 0.30;
}

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

struct Record* record_new(uint32_t id, const char* destination, const char* departure_datetime, const char* arrival_datetime, const char* type_of_coach, double ticket_price, const char* purchase_datetime, bool available) {
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

struct RecordQueue {
	struct Record* head;
	struct Record* tail;
	uint32_t nextID;
};

// Add the item to the end
void queue_push(struct RecordQueue* q, const char* destination, const char* departure_datetime, const char* arrival_datetime, const char* type_of_coach, double ticket_price, const char* purchase_datetime, bool available) {
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

// Global variables
struct RecordQueue purchases = { .head = NULL, .tail = NULL, .nextID = 1 };
struct RecordQueue returns = { .head = NULL, .tail = NULL, .nextID = 1 };
struct Timestamp current_date = { .hours = 11, .minutes = 30, .day = 1, .month = 1, .year = 1970 };

// UTIL FUNCTIONS
// TODO: get get_valid_option and clear_buffer functions to a separate io.h file
// buffer clear function to check for an Invalid input and clear the whole line if the input is Invalid
void clear_buffer() {
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
}

// Safe input extraction protected from buffer overflow
const char* get_string_input(int max_len) {
	char* buffer = malloc(sizeof(char) * (max_len + 1));

	// max_len + 1 for null-termination
	if (fgets(buffer, max_len + 1, stdin) == NULL) {
		return NULL;
	}

	// Set '\0' where '\n' is located to handle ENTER keypress
	buffer[strcspn(buffer, "\n")] = '\0';

	// If our buffer is allocated but length is longer than it should be, free the buffer and return NULL
	if (buffer && (int)strlen(buffer) >= max_len) {
		free(buffer);
		return NULL;
	}

	return buffer;
}

double get_double_input(void) {
	double input;
	if (scanf("%lf", &input) != 1) {
		printf("Invalid input.\n");
		clear_buffer();
		return -1.0;
	}
	clear_buffer();
	return input;
}

// Get an in-bounds integer input
int get_valid_option(int option_min, int option_max) {
    int option;
    while (true) {
        printf("Your option: ");

        // If input is a non-integer or it's smaller than the minimum option or it's larger that the maximum option, try again
        if (scanf("%d", &option) != 1 || option < option_min || option > option_max) {
            printf("Invalid input. Valid options are %d-%d.\n", option_min, option_max);
            clear_buffer();
            continue;
        }

        // If our check doesn't pass, break the loop and return the result
        break;
    }
    return option;
}

// subemnus
void purchase_submenu_process() {
	while (true) {
		printf("--------PURCHASES-------\n");
		printf("1. Put a new purchase record.\n");
		printf("2. Drop the latest purchase record.\n");
		printf("3. Clear the purchase records.\n");
		printf("4. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 4);

		if (option == 1) {
			clear_buffer();
			printf("Destination: ");
			const char* destination = get_string_input(60);

			printf("Departing (hh:mm-DD/MM/YYYY): ");
			const char* departure_datetime = get_string_input(20);

			printf("Arriving (hh:mm-DD/MM/YYYY): ");
			const char* arrival_datetime = get_string_input(20);

			printf("Type of Coach: ");
			const char* type_of_coach = get_string_input(25);

			printf("Ticket Price: ");
			double ticket_price = get_double_input();

			// In future will be replaced with automatic input based on current datetime
			printf("Purchase Time (hh:mm-DD/MM/YYYY): ");
			const char* purchase_datetime = get_string_input(20);

			printf("Available (yes/no): ");
			bool available;
			const char* available_str = get_string_input(4);
			available = strcmp(available_str, "yes") == 0;

			queue_push(&purchases, destination, departure_datetime, arrival_datetime, type_of_coach, ticket_price, purchase_datetime, available);
			printf("Successfully added this record to the purchases queue!\n");
		} else if (option == 2) {
			queue_pop(&purchases);
		} else if (option == 3) {
			queue_clear(&purchases);
		} else if (option == 4) {
			break;
		}
	}
}

void return_submenu_process() {
	while (true) {
		printf("---------RETURNS--------\n");
		printf("1. Put a new return record.\n");
		printf("2. Drop the latest return record.\n");
		printf("3. Clear the return records.\n");
		printf("4. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 4);

		if (option == 1) {
			clear_buffer();
			printf("Destination: ");
			const char* destination = get_string_input(60);

			printf("Departing (hh:mm-DD/MM/YYYY): ");
			const char* departure_datetime = get_string_input(20);

			printf("Arriving (hh:mm-DD/MM/YYYY): ");
			const char* arrival_datetime = get_string_input(20);

			printf("Type of Coach: ");
			const char* type_of_coach = get_string_input(25);

			printf("Ticket Price: ");
			double ticket_price = get_double_input();

			// In future will be replaced with automatic input based on current datetime
			printf("Purchase Time (hh:mm-DD/MM/YYYY): ");
			const char* purchase_datetime = get_string_input(20);

			printf("Available (yes/no): ");
			bool available;
			const char* available_str = get_string_input(4);
			available = strcmp(available_str, "yes") == 0;

			queue_push(&returns, destination, departure_datetime, arrival_datetime, type_of_coach, ticket_price, purchase_datetime, available);
			printf("Successfully added this record to the returns queue!\n");
		} else if (option == 2) {
			queue_pop(&returns);
		} else if (option == 3) {
			queue_clear(&returns);
		} else if (option == 4) {
			break;
		}
	}
}

void report_submenu_process() {
	while (true) {
		printf("----------REPORT--------\n");
		printf("Current date is: ");
		timestamp_print(current_date);
		printf("\n");
		printf("1. List all purchase records.\n");
		printf("2. See the first purchase record.\n");
		printf("3. See the last purchase record.\n");
		printf("4. List all return records.\n");
		printf("5. List the first return record.\n");
		printf("6. List the last return record.\n");
		printf("7. Print all records.\n");
		printf("8. Drop all records.\n");
		printf("9. Set current date.\n");
		printf("10. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 10);
		if (option == 1) {
			queue_print(purchases);
		} else if (option == 2) {
			if (!queue_is_empty(purchases)) {
				record_print(purchases.head);
			} else {
				printf("The queue is empty!\n");
			}
		} else if (option == 3) {
			if (!queue_is_empty(purchases)) {
				record_print(purchases.tail);
			} else {
				printf("The queue is empty!\n");
			}
		} else if (option == 4) {
			queue_print(returns);
		} else if (option == 5) {
			if (!queue_is_empty(returns)) {
				record_print(returns.head);
			} else {
				printf("The queue is empty!\n");
			}
		} else if (option == 6) {
			if (!queue_is_empty(returns)) {
				record_print(returns.tail);
			} else {
				printf("The queue is empty!\n");
			}
		} else if (option == 7) {
			printf("PURCHASES:\n");
			queue_print(purchases);
			printf("RETURNS:\n");
			queue_print(returns);
		} else if (option == 8) {
			queue_clear(&returns);
			queue_clear(&purchases);
		} else if (option == 9) {
			clear_buffer();
			printf("Date: ");
			const char* date = get_string_input(20);
			timestamp_from_string(date, &current_date);
		} else if (option == 10) {
			break;
		}
	}
}

void main_loop() {
    while (true) {
		printf("-----------MAIN---------\n");
		printf("1. Manage purchases.\n");
		printf("2. Manage returns.\n");
		printf("3. Manage the current day report.\n");
		printf("4. Quit.\n");
		printf("------------------------\n");

        int option = get_valid_option(1, 4);

        // main menu options
        // Get the input until user hits 1 - "Return to the main menu"
        if (option == 1) {
            purchase_submenu_process();
        } else if (option == 2) {
            return_submenu_process();
        } else if (option == 3) {
            report_submenu_process();
        } else if (option == 4) {
            printf("Goodbye!\n");
            break; // Quit the main loop
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main() {
    main_loop();
    return 0;
}
