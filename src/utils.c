#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool is_leap_year(uint16_t year) {
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

uint8_t get_days_from_month(uint8_t month, uint16_t year) {
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

// Safely get double from user
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


