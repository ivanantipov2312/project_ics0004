#include <stdint.h>
#include <stdio.h>
// use true to distinguish true value from integer literal 1
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

// Global queues for storing data
struct RecordQueue purchases = { .head = NULL, .tail = NULL, .nextID = 1 };
struct RecordQueue returns = { .head = NULL, .tail = NULL, .nextID = 1 };

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
		clear_buffer();
		return 0.0;
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

			printf("Departing (YYYY-MM-DD-hh-mm): ");
			const char* departure_datetime = get_string_input(20);

			printf("Arriving (YYYY-MM-DD-hh-mm): ");
			const char* arrival_datetime = get_string_input(20);

			printf("Type of Coach: ");
			const char* type_of_coach = get_string_input(25);

			printf("Ticket Price: ");
			double ticket_price = get_double_input();

			// In future will be replaced with automatic input based on current datetime
			printf("Purchase Time (YYYY-MM-DD-hh-mm): ");
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

		printf("Departing (YYYY-MM-DD-hh-mm): ");
		const char* departure_datetime = get_string_input(20);

		printf("Arriving (YYYY-MM-DD-hh-mm): ");
		const char* arrival_datetime = get_string_input(20);

		printf("Type of Coach: ");
		const char* type_of_coach = get_string_input(25);

		printf("Ticket Price: ");
		double ticket_price = get_double_input();

		// In future will be replaced with automatic input based on current datetime
		printf("Purchase Time (YYYY-MM-DD-hh-mm): ");
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
		printf("1. List all purchase records.\n");
		printf("2. See the first purchase record.\n");
		printf("3. See the last purchase record.\n");
		printf("4. List all return records.\n");
		printf("5. List the first return record.\n");
		printf("6. List the last return record.\n");
		printf("7. Print all records.\n");
		printf("8. Drop all records.\n");
		printf("9. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 9);
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
