#include <stdio.h>

void print_purchase_submenu() {
    printf("*******************\n");
    printf("Purchases Submenu - Under Construction\n");
    printf("*******************\n");
}

void print_return_submenu() {
    printf("*******************\n");
    printf("Returns Submenu - Under Construction\n");
    printf("*******************\n");
}

void print_report_submenu() {
    printf("*******************\n");
    printf("Report Submenu - Under Construction\n");
    printf("*******************\n");
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
    while (1) {
        print_menu();
        
        printf("Your option: ");
        int option = 0;
        
        if (scanf("%d", &option) != 1) {
            printf("Invalid input. Valid options are 1-4!\n");
            clear_buffer();
			continue;
		}

        if (option == 1) {
            print_purchase_submenu();
        } else if (option == 2) {
            print_return_submenu();
        } else if (option == 3) {
            print_report_submenu();
        } else if (option == 4) {
            break;
        } else {
            printf("Invalid input. Valid options are 1-4!\n");
        }
    }
}

int main(void) {
    main_loop();
    return 0;
}
