#include <stdio.h>

// Menu tracking
int in_menu = 1;
int in_purchase = 0;
int in_return = 0;
int in_report = 0;
int is_on = 1;

void print_purchase_submenu() {
    printf("--------PURCHASES-------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_return_submenu() {
    printf("---------RETURNS--------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_report_submenu() {
    printf("----------REPORT--------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

// main menu
void print_menu() {
    printf("-----------MAIN---------\n");
    printf("1. See the purchases.\n");
    printf("2. See the returns.\n");
    printf("3. Manage the current day report.\n");
    printf("4. Quit.\n");
    printf("------------------------\n");
}

// buffer clear function to fix bug with non-integer input
void clear_buffer() {
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
}

// Get an in-bounds integer input
int get_valid_option(int option_min, int option_max) {
    int option;
    while (1) {
        printf("Your option: ");

        // If input is not an integer or it is smaller than minimum option or it is bigger that maximum option, continue
        if (scanf("%d", &option) != 1 || option < option_min || option > option_max) {
            printf("Invalid input. Valid options are %d-%d.\n", option_min, option_max);
            clear_buffer();
            continue;
        }

        // If our check doesn't pass, break the loop
        break;
    }
    return option;
}

void main_loop() {
    while (1) {
        print_menu();
        int option = get_valid_option(1, 4);

        // main menu options
        if (option == 1) {
            print_purchase_submenu();
            while ((option = get_valid_option(1, 1)) != 1);
        } else if (option == 2) {
            print_return_submenu();
            while ((option = get_valid_option(1, 1)) != 1);
        } else if (option == 3) {
            print_report_submenu();
            while ((option = get_valid_option(1, 1)) != 1);
        } else if (option == 4) {
            printf("Goodbye!\n");
            break;
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main(void) {
    main_loop();
    return 0;
}
