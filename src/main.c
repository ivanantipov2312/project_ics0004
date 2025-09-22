#include <stdio.h>

// Submenus currently WIP, trying to access will just print the submenu WIP text and throw back to main menu
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

// Main menu
void print_menu() {
    printf("------------------------\n");
    printf("1. See the purchases.\n");
    printf("2. See the returns.\n");
    printf("3. Manage the current day report.\n");
    printf("4. Quit.\n");
    printf("------------------------\n");
}

// Function to clear buffer; made to fix bug with non-integer input in menu navigation
void clear_buffer() {
    int buffer;
    while ((buffer = getchar()) != '\n' && buffer != EOF);
}

void main_loop() {
    while (1) {
        print_menu();
        
        printf("Your option: ");
        int option = 0;
        
        // Checks if scanf input is integer, if not prints error and goes to clear buffer to avoid infitely looping menu print bug
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
            break;  // Quit main_loop and return 0 as shown in 'int main(void)'
        } else {
            printf("Invalid input. Valid options are 1-4!\n");
        }
    }
}

// Run main_loop function, will break if option 4 "Quit" is chosen within the loop and return 0
int main(void) {
    main_loop();
    return 0;
}
