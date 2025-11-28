#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "queue.h"
#include "utils.h"
#include "timestamp.h"
#include "file.h"

// Global variables
struct RecordQueue purchases = { .head = NULL, .tail = NULL, .nextID = 1 };
struct RecordQueue returns = { .head = NULL, .tail = NULL, .nextID = 1 };
struct Timestamp current_date = { .hours = 11, .minutes = 30, .day = 1, .month = 1, .year = 1970 };

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
			float ticket_price = get_float_input();

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
			float ticket_price = get_float_input();

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
		printf("10. Save the report to a file.\n");
		printf("11. Read the report from a file.\n");
		printf("12. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 12);
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
			// Clean up our queues
			queue_clear(&purchases);
			queue_clear(&returns);
            break; // Quit the main loop
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main() {
    main_loop();
    return 0;
}
