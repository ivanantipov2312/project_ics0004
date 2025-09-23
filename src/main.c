#include <stdio.h>

#include <stdio.h>

int in_menu = 1;
int in_purchase = 0;
int in_return = 0;
int in_report = 0;
int is_on = 1;

void print_purchase_submenu() {
    printf("*******************\n");
    printf("Purchases Submenu - Under Construction\n");
    printf("*******************\n");
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_return_submenu() {
    printf("*******************\n");
    printf("Returns Submenu - Under Construction\n");
    printf("*******************\n");
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_report_submenu() {
    printf("*******************\n");
    printf("Report Submenu - Under Construction\n");
    printf("*******************\n");
    printf("------------------------\n");
    printf("1. Go back to menu.\n");
    printf("------------------------\n");
}

void print_menu() {
    printf("------------------------\n");
    printf("1. See the purchases.\n");
    printf("2. See the returns.\n");
    printf("3. Manage the current day report.\n");
    printf("4. Quit.\n");
    printf("------------------------\n");
}

void clear_buffer() {
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
}

void main_loop() {
    while (is_on == 1) {
        while (in_menu == 1) {
            print_menu();

            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. Valid options are 1-4!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                in_menu = 0;
                in_purchase = 1;
            }
            else if (option == 2) {
                in_menu = 0;
                in_return = 1;
            }
            else if (option == 3) {
                in_menu = 0;
                in_report = 1;
            }
            else if (option == 4) {
                is_on = 0;
                printf("Goodbye");
                break;
            }
            else {
                printf("Invalid input. Valid options are 1-4!\n");
            }
        }

        while (in_purchase == 1) {
            print_purchase_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                in_menu = 1;
                in_purchase = 0;
            }
            else {
                printf("Invalid input. The only valid option is 1!\n");
            }
        }

        while (in_return == 1) {
            print_return_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                in_return = 0;
                in_menu = 1;
            }
        }
        while (in_report == 1) {
            print_report_submenu();
            printf("Your option: ");
            int option = 0;

            if (scanf("%d", &option) != 1) {
                printf("Invalid input. The only valid option is 1!\n");
                clear_buffer();
                continue;
            }

            if (option == 1) {
                in_report = 0;
                in_menu = 1;
            }
        }
    }
}
int main(void) {
    main_loop();
    return 0;
}