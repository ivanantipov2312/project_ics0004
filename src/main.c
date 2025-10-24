#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ticket {
    int ID;                 // record ID
    bool lock;              // locked while viewing
    float price;            // price of ticket
    char type[10];          // cabin type
    char destination[10];   // destination
    char date[11];          // date of travel
    char passport[20];      // passport number
};

// global ticket database
// TODO: hardcoded size
struct Ticket ticketDB[] = {
    { 100, false, 20.25, "sleeper\0", "Seattle\0", "10.04.2026\0", '\0' },
    { 101, false, 20.25, "dining\0", "Seattle\0", "10.04.2026\0", '\0' },
    { 102, false, 20.25, "luxury\0", "Seattle\0", "10.04.2026\0", '\0' },
    { 103, false, 20.25, "sleeper\0", "Timbuktu\0", "10.04.2026\0", '\0' },
    { 104, false, 20.25, "dining\0", "Timbuktu\0", "10.04.2026\0", '\0' },
    { 105, false, 20.25, "luxury\0", "Timbuktu\0", "10.04.2026\0", '\0' },
    { 106, false, 20.25, "sleeper\0", "London\0", "10.04.2026\0", '\0' },
    { 107, false, 20.25, "dining\0", "London\0", "10.04.2026\0", '\0' },
    { 108, false, 20.25, "luxury\0", "London\0", "10.04.2026\0", '\0' },
    { 109, false, 20.25, "sleeper\0", "Tartu\0", "10.04.2026\0", '\0' }
};

// global daily ticket sales
// TODO: hardcoded size
struct Ticket dailySales[10] = { 0 };

void query(struct Ticket *queryTicket, struct Ticket* results[]) {

    // if querying a specific ticket ID, must be a refund, otherwise purchase
    if (queryTicket->ID > 0) {
        printf("This is a RETURN request!\n");
    } else {
        printf("This is a PURCHASE request!\n");

        int resultCount = 0;

        for (int i = 0; i < 10; i++) {

            // nested just because it looked better than a long comparison ...
            if (strcmp(queryTicket->destination, ticketDB[i].destination) == 0){
                if (strcmp(queryTicket->date, ticketDB[i].date) == 0) {
                    if (strcmp(queryTicket->type, ticketDB[i].type) == 0) {
                        printf("It's a match! (%d)\n", resultCount);
                        results[resultCount] = &ticketDB[i];
                        resultCount++;
                    }
                }
            }

            if (strcmp(queryTicket->type, ticketDB[i].type) && strcmp(queryTicket->destination, ticketDB[i].destination) && strcmp(queryTicket->date, ticketDB[i].date)) {
                printf("It's a match!\n");
                // add to results[i]
                resultCount++;
            }
        }
    }

}

/*
void register_sale(struct Ticket *ticket) {
    // call from update? or from purchase?
    // add Ticket to sales array
}
*/

void purchase() {
    // create Ticket struct
    struct Ticket queryTicket = { 0 };

    // input destination
    char dest[10];
    printf("Destination: ");
    scanf("%s", queryTicket.destination);

    // input date
    char date[11];
    printf("Date (DD.MM.YYYY): ");
    scanf("%s", queryTicket.date);

    // input type
    char type[10];
    printf("Type (sleeper/dining/luxury): ");
    scanf("%s", queryTicket.type);

    // TODO: fix hardcoded size
    struct Ticket* results[10];
    // query our desired ticket and link any finds into our results array
    query(&queryTicket, results);

    printf("ID: %s", results[0]->ID);

    // ask, input customer selection

    // input passport (update the Ticket pointer in ticketDB)

    // ask simple confirmation

    //update appropriate Ticket* in ticketDB with passport info

    // unlock tickets (iterate through results array and unset lock)
}

void refund() {
    // input ticket ID
    int ticketID;
    printf("Return Ticket ID (###): ");
    scanf("%d", &ticketID);

    // create Ticket struct
    struct Ticket queryTicket = { ticketID, false, 0.0, '\0', '\0', '\0', '\0' };
    struct Ticket result = { 0, false, 0.0, '\0', '\0', '\0', '\0' };

    /*
    query(&queryTicket, &result);

    if (result.ID > 0) {
        // calculate refund here
    }
    */

    // ask confirmation

    // set Ticket->passport to empty again
    //update(Ticket*);
}

void report() {
    // just iterate through dailySales[] 0-10 and print any non-zero entries, stopping when hits 0
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

void main_loop() {
    while (true) {

        print_menu();

        printf("Your option: ");
        int option;
        scanf("%d", &option);
        while ((getchar()) != '\n');

        switch (option) {
            case 1:
                purchase();
                break;
            case 2:
                refund();
                break;
            case 3:
                report();
                break;
            case 4:
                printf("Goodbye!\n");
                exit(0);
        }
    }
}

// goes to main loop, returns 0 if loop is broken with input "4" in main menu
int main() {
    main_loop();
    return 0;
}
