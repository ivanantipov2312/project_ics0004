#include <stdio.h>

// Menu tracking
int menu = 1;

void print_purchase_submenu() {
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_return_submenu() {
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_report_submenu() {
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

// main menu
void print_menu() {
    printf("------------------------\n");
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

void main_loop() {
    while (menu != 0) {
        while (menu == 1) {
            print_menu();

            printf("Your option: ");
            int option = 0;

            // If input isn't integer - clears buffer; part of non-integer input bugfix
            if (scanf("%d", &option) != 1) {
                printf("Invalid input. Valid options are 1-4!\n");
                clear_buffer();
                continue;
            }
            // main menu options
            if (option == 1) {
                menu = 2;
            }
            else if (option == 2) {
                menu = 3;
            }
            else if (option == 3) {
                menu = 3;
            }
            else if (option == 4) {
                menu = 0;
                printf("Goodbye");
                break;
            }
            else {
                printf("Invalid input. Valid options are 1-4!\n");
            }
        }

        // purchase submenu
        while (menu == 2) {
            print_purchase_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                menu = 1;
            }
            else {
                printf("Invalid input. The only valid option is 1!\n");
            }
        }

        // returns submenu
        while (menu == 3) {
            print_return_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                menu = 1;
            }
            else {
                printf("Invalid input. The only valid option is 1!\n");
            }
        }

        // report submenu
        while (menu == 4) {
            print_report_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                menu = 1;
            }
            else {
                printf("Invalid input. The only valid option is 1!\n");
            }
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main(void) {
    main_loop();
    return 0;
}