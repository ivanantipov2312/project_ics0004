#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "queue.h"
#include "utils.h"
#include "timestamp.h"
#include "file.h"

// Global variables
struct RecordQueue database = { .head = NULL, .tail = NULL, .nextID = 1 };
struct RecordQueue purchases = { .head = NULL, .tail = NULL, .nextID = 1 };
struct RecordQueue returns = { .head = NULL, .tail = NULL, .nextID = 1 };
struct Timestamp current_date = { .hours = 9, .minutes = 37, .day = 28, .month = 9, .year = 2025 };

void process_purchase() {
	clear_buffer();

	// take desired customer input for destination, date, and coach type
	char* destination = get_string_input("Destination: ", 60);
	char* departure_datetime = get_string_input("Departing (DD/MM/YYYY hh:mm): ", 18); // 16 for date + 1 for '\n' + 1 for '\0'
	char* type_of_coach = get_string_input("Type of Coach: ", 25);

	// search ticket queue for matching record
	struct Record* match = NULL;
	queue_search(&match, database, destination, departure_datetime, type_of_coach, NULL);

	if (!match) {
		printf("\n> No matching tickets available ...\n\n");
		return;
	}

	// print matching record for validation
	printf("\n🔵 %s\t", match->destination);
	timestamp_print(match->departure_timestamp);
	printf("\t%s\t%.2f\n\n", match->type_of_coach, match->ticket_price);

	// confirm purchase
	printf("1. ✅ Yes\n");
	printf("2. ⛔ No\n");
	printf("Confirm? ");

	int option = get_valid_option(1, 2);

	if (option == 1) {
		match->available = false;
	} else if (option == 2) {
		return;
	}

	printf("\n> Ticket has been marked sold, updated to unavailable\n");
	printf("> Please receive customer funds for %.2f\n", match->ticket_price);
	printf("> Enter Passport ID in form of 3 letter country code + 8 digit ID code\n\n");

	char* passport_id = get_string_input("Passport ID (CCC########): ", 12);
	match->passport_id = passport_id;

	// add entry to purchases queue
	queue_push(&purchases, match->destination, departure_datetime, match->type_of_coach, match->ticket_price, false, match->passport_id);

	free(destination);
	free(departure_datetime);
	free(type_of_coach);
}

void process_return() {
	char* passport_id = get_string_input("Passport ID (CCC########): ", 12);

	// search ticket queue for matching record
	struct Record* match = NULL;
	queue_search(&match, database, NULL, NULL, NULL, passport_id);

	if (!match) {
		printf("\n> No matching sales records located ...\n\n");
		return;
	}

	record_print(match);

	// calculate penalty
	float penalty = timestamp_penalty(current_date, match->departure_timestamp);
	float return_value = match->ticket_price - (match->ticket_price * penalty);

	printf("\n> Customer is entitled to a return of %.2f\n\n", return_value);

	printf("1. ✅ Yes\n");
	printf("2. ⛔ No\n");
	printf("Confirm? ");

	int option = get_valid_option(1, 2);

	if (option == 1) {
		// mark ticket available for sale again
		match->available = true;
	} else if (option == 2) {
		return;
	}

	char* departure_datetime = timestamp_to_string(match->departure_timestamp);

	// add entry to returns queue
	queue_push(&returns, match->destination, departure_datetime, match->type_of_coach, return_value, true, match->passport_id);

	// null out Passport ID information
	match->passport_id = "NNN00000000\0";
}

// subemnus
void purchase_submenu_process() {
	while (true) {
		printf("--------PURCHASES-------\n");
		printf("1. ℹ️ List Available\n");
		printf("2. 💳 Enter Information\n");
		printf("3. ↩️ Back\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 3);

		if (option == 1) {
			printf("Tickets: \n");
			queue_print(database, 1);
		} else if (option == 2) {
			process_purchase();
		} else if (option == 3) {
			break;
		}
	}
}

void return_submenu_process() {
	while (true) {
		printf("---------RETURNS--------\n");
		printf("1. 🛂 Enter Information\n");
		printf("2. ↩️ Back\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 2);

		if (option == 1) {
			process_return();
		} else if (option == 2) {
			break;
		}
	}
}

void report_submenu_process() {
	while (true) {
		printf("----------REPORT--------\n");
		printf("🕛 Current date is: ");
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
		printf("10. Save the report to a file.\n");
		printf("11. Read the report from a file.\n");
		printf("12. Back\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 12);
		if (option == 1) {
			printf("Purchases: \n");
			queue_print(purchases, 0);
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
			printf("Returns: \n");
			queue_print(returns, 0);
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
			printf("Purchases:\n");
			queue_print(purchases, 0);
			printf("Returns:\n");
			queue_print(returns, 0);
		} else if (option == 8) {
			queue_clear(&returns);
			queue_clear(&purchases);
			printf("Both queues are cleared out!\n");
		} else if (option == 9) {
			clear_buffer();
			const char* date = get_string_input("Date: ", 20);
			timestamp_from_string(date, &current_date);
		} else if (option == 10) {
			printf("Writing to purchases.csv...\n");
			file_write(purchases, "purchases.csv");
			printf("Writing to returns.csv...\n");
			file_write(returns, "returns.csv");
		} else if (option == 11) {
			// Clear the queues to avoid duplicate readings
			queue_clear(&purchases);
			queue_clear(&returns);

			printf("Reading purchases.csv...\n");
			file_read(&purchases, "purchases.csv");
			printf("Reading returns.csv...\n");
			file_read(&returns, "returns.csv");
			continue;
		} else if (option == 12) {
			break;
		}
	}
}

void main_loop() {
    while (true) {
		printf("-----------MAIN---------\n");
		printf("1. 💲 Make Purchase\n");
		printf("2. 🫲 Process Return\n");
		printf("3. 📃 Admin Reporting\n");
		printf("4. 🛑 Quit\n");
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
			// Clean up our queues
			queue_clear(&database);
			queue_clear(&purchases);
			queue_clear(&returns);
            break; // Quit the main loop
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main() {
	// load default sample set of available tickets
	file_read(&database, "default.csv");
	main_loop();
	return 0;
}
