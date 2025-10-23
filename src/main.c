#include <stdio.h>
// use true to distinguish true value from integer literal 1
#include <stdbool.h>

// UTIL FUNCTIONS
// TODO: get get_valid_option and clear_buffer functions to a separate io.h file
// buffer clear function to check for an Invalid input and clear the whole line if the input is Invalid
void clear_buffer() {
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
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
		printf("2. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 2);

		if (option == 1) {

		} else if (option == 2) {
			break;
		}
	}
}

void return_submenu_process() {
	while (true) {
		printf("---------RETURNS--------\n");
		printf("1. Put a new return record.\n");
		printf("2. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 2);
		if (option == 1) {
		} else if (option == 2) {
			break;
		}
	}
}

void report_submenu_process() {
	while (true) {
		printf("----------REPORT--------\n");
		printf("1. List all purchase records.\n");
		printf("2. List all return records.\n");
		printf("3. List all records.\n");
		printf("4. Go back to menu.\n");
		printf("------------------------\n");

		int option = get_valid_option(1, 4);
		if (option == 1) {

		} else if (option == 2) {

		} else if (option == 3) {

		} else if (option == 4) {
			break;
		}
	}
}

void main_loop() {
    while (true) {
		printf("-----------MAIN---------\n");
		printf("1. See the purchases.\n");
		printf("2. See the returns.\n");
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
