#include <stdint.h>
#include <stdio.h>
// use true to distinguish true value from integer literal 1
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Record {
	uint32_t id;
	char* destination;
	char* date;
	char* type_of_coach;
	bool available;
	struct Record* next; // For queue
};

struct Record* record_new(uint32_t id, const char* destination, const char* date, const char* type_of_coach, bool available) {
	struct Record* rec = malloc(sizeof(*rec));
	rec->id = id;
	rec->available = available;

	// Write data to the structure
	// len + 1 because snprintf adds null-termination
	size_t len = strlen(destination);
	rec->destination = malloc(sizeof(char) * (len + 1));
	snprintf(rec->destination, len + 1, "%s", destination);
	rec->destination[len] = '\0';

	len = strlen(date);
	rec->date = malloc(sizeof(char) * (len + 1));
	snprintf(rec->date, len + 1, "%s", date);
	rec->date[len] = '\0';

	const char* type_ptr = type_of_coach ? type_of_coach : "Unspecified";
	len = strlen(type_ptr);
	rec->type_of_coach = malloc(sizeof(char) * (len + 1));
	snprintf(rec->type_of_coach, len + 1, "%s", type_ptr);
	rec->type_of_coach[len] = '\0';

	rec->next = NULL;

	return rec;
}

void record_free(struct Record* rec) {
	if (rec) {
		rec->id = 0;
		rec->available = false;

		if (rec->destination) {
			free(rec->destination);
			rec->destination = NULL;
		}

		if (rec->date) {
			free(rec->date);
			rec->date = NULL;
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
	printf("ID: %u, Destination: %s, Date: %s, Type of Coach: %s, Available: %s\n",
			rec->id, rec->destination, rec->date, rec->type_of_coach, rec->available ? "Yes": "No");
}

struct RecordQueue {
	struct Record* head;
	struct Record* tail;
};

// Add the item to the end
void queue_push(struct RecordQueue* q, uint32_t id, const char* destination, const char* date, const char* type_of_coach, bool available) {
	if (!q->head) {
		q->head = q->tail = record_new(id, destination, date, type_of_coach, available);
		return;
	}

	struct Record* tmp = record_new(id, destination, date, type_of_coach, available);
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
struct RecordQueue purchases = { .head = NULL, .tail = NULL };
struct RecordQueue returns = { .head = NULL, .tail = NULL };

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
			printf("ID: ");
			uint32_t id;
			sscanf(get_string_input(10), "%u", &id);
			clear_buffer();
			printf("Destination: ");
			const char* destination = get_string_input(60);
			printf("Date: ");
			const char* date = get_string_input(16);
			printf("Type of Coach: ");
			const char* type_of_coach = get_string_input(25);
			printf("Available (yes/no): ");
			bool available;
			const char* available_str = get_string_input(4);
			available = strcmp(available_str, "yes") == 0;
			queue_push(&purchases, id, destination, date, type_of_coach, available);
			printf("Successfully added this record to the queue!\n");
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
			printf("ID: ");
			uint32_t id;
			const char* id_str = get_string_input(10);
			sscanf(id_str, "%u", &id);
			printf("ID: %u\n", id);

			printf("Destination: ");
			const char* destination = get_string_input(60);

			printf("Date: ");
			const char* date = get_string_input(16);

			printf("Type of Coach: ");
			const char* type_of_coach = get_string_input(25);

			printf("Available (yes/no): ");
			bool available;
			const char* available_str = get_string_input(4);
			available = strcmp(available_str, "yes") == 0;

			queue_push(&returns, id, destination, date, type_of_coach, available);
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
		printf("5. List the first purchase record.\n");
		printf("6. List the last purchase record.\n");
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
