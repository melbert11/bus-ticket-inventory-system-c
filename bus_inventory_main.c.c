/*BUS TICKETING SYSTEM
Members: Melbert Bonina
         Maria Francia Abdula
         Rhoda Rodriguez
         Angelica Bilaos
*/

#include <stdio.h>
#include <string.h>

#define MAX_LEN 100
#define MAX_ROUTE 50
#define MAX_CATEGORY 10
#define MAX_PASSENGERS 1000
#define MAX_OPERATOR 25

typedef struct
{
    long int idNumber;
    char firstName[MAX_LEN];
    char lastName[MAX_LEN];
    int pin;
} Operator;

typedef struct
{
    int routeRef;
    char destination[MAX_LEN];
    float distanceKm;
    char status[MAX_LEN];
} Route;

typedef struct
{
    int priceRef;
    char busType[MAX_LEN];
    float pricePhpKm;
} Price;

typedef struct
{
    int passengerID;
    char name[MAX_LEN];
    char destination[MAX_LEN];
    char busType[MAX_LEN];
    float ticketPrice;
    int ticketsNum;
    int regTickets;
    int discountTickets;
    double totalPayment;
} Passenger;

typedef struct
{
    long int ticketsNumTotal;
    long int regularTicketsTotal;
    long int discountTicketsTotal;
    double totalSales;
    long int ticketCountRegular[MAX_ROUTE][MAX_CATEGORY];
    long int ticketCountDiscount[MAX_ROUTE][MAX_CATEGORY];
} Summary;

void calculatePayment(Passenger *passengerInfo, Price *priceList, Route *routeList, float discountRate, int routeIdChoice, int numOfRoutes, int numOfPrice);

void printSummary(FILE *summaryFile, Summary *summaryPassenger, int numOfRoutes, int numOfPrice, char *date, int numOfPassenger, Route *routeList, Price *priceList);
void countTicketsPerRoute(Passenger *passenger, Summary *summaryPassenger, Route *routeList, Price *priceList, int numOfRoutes, int numOfPrice);
void summarizePassengerList(Passenger *passenger, Summary *summaryPassenger);
void printTicket(Passenger *passengerInfo, FILE *ticketFile, int routeId, char *origin, char *date);

int searchOperator(Operator *operatorCheck, Operator *operatorList, int numOfOperator);
int searchRoute(Route *routeList, int routeIdCheck, int numOfRoutes);
int searchCategory(Price *priceList, int priceIdCheck, int numOfPrice);
int findDestinationByRouteId(Route *routeList, int numOfRoutes, int routeIdChoice, char *passengerDestination);

void swapRoute(Route *route1, Route *route2);
void swapCategory(Price *price1, Price *price2);

void addTicket(long int *numOfBookings, Passenger *bookingList, Passenger *passengerInfo);

void readBookingInput(FILE *bookingInputFile, Passenger *bookingList, long int numOfBookings);
void readPriceFile(FILE *pricingFile, Price *priceList, int numOfPrice);
void readRouteFile(FILE *routeFile, Route *routeList, int numOfRoutes);
void readOperatorFile(FILE *operatorFile, Operator *operatorList, int numOfOperator);

void displayRoute(Route *routeList, int numOfRoutes);
void displayPrice(Price *priceList, int numOfPrice);
void displayOperatorRoute(Route *routeList, int numOfRoutes);
void updatePrice(FILE *priceFile, int numOfPrice, Price *priceList);
void updateRoute(FILE *routeFile, int numOfRoute, Route *routeList);
void updateBookings(FILE *bookingInputUpdateFile, FILE *bookingOutputFile, long int numOfBookings, Passenger *bookingList);
void displayInstruction();
void displayFAQs();

int main()
{

    FILE *operatorFile;
    operatorFile = fopen("operators.txt", "r");
    Operator operatorList[MAX_OPERATOR];
    int numOfOperator;
    if (operatorFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    else
    {
        fscanf(operatorFile, "%d", &numOfOperator);
        readOperatorFile(operatorFile, operatorList, numOfOperator);
    }

    FILE *routeFile; // Route File
    routeFile = fopen("route.txt", "r");
    Route routeList[MAX_ROUTE];
    int numOfRoutes;
    if (routeFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    else
    {
        fscanf(routeFile, "%d", &numOfRoutes);
        readRouteFile(routeFile, routeList, numOfRoutes);
    }

    FILE *pricingFile;
    pricingFile = fopen("price.txt", "r");
    Price priceList[MAX_CATEGORY];
    int numOfPrice;
    if (pricingFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    else
    {
        fscanf(pricingFile, "%d", &numOfPrice);
        readPriceFile(pricingFile, priceList, numOfPrice);
    }

    FILE *ticketFile;
    ticketFile = fopen("tickets.txt", "w");
    if (ticketFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }

    FILE *bookingInputFile;
    bookingInputFile = fopen("passenger.in", "r");
    Passenger bookingList[MAX_PASSENGERS];
    long int numOfBookings;
    if (bookingInputFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    else
    {
        fscanf(bookingInputFile, "%ld", &numOfBookings);
        readBookingInput(bookingInputFile, bookingList, numOfBookings);
    }

    FILE *bookingUpdateFile;
    bookingUpdateFile = fopen("passenger.in", "w");
    if (bookingUpdateFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }

    FILE *bookingOutputFile;
    bookingOutputFile = fopen("passenger.out", "w");
    if (bookingOutputFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    // print Table header
    fprintf(bookingOutputFile, "==============================================================================================================================================================\n");
    fprintf(bookingOutputFile, "| %-9s | %-18s | %-18s | %-18s | %-19s | %-15s | %-8s | %-11s | %-14s |\n", "Ref. No.", "Name", "Route-Destination", "Bus Type", "Ticket Price (PHP)", "No. of Tickets", "Regular", "Discounted", "Total Payment");
    fprintf(bookingOutputFile, "==============================================================================================================================================================\n");
    FILE *routeUpdateFile;
    routeUpdateFile = fopen("route.txt", "w");
    if (routeUpdateFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }
    FILE *priceUpdateFile;
    priceUpdateFile = fopen("price.txt", "w");
    if (priceUpdateFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }

    FILE *summaryFile;
    summaryFile = fopen("summary.txt", "w");
    if (summaryFile == NULL)
    {
        printf("Error opening file\n");
        return 1; // Exit program with error code
    }

    char mainOption, bookingOption, printOption, operatorOption, priceOption, editOption, routeOption;
    int searchResult, passResult, busTypeChoice, routeIdChoice, priceIdChoice;

    char origin[MAX_LEN], date[MAX_LEN]; // Date and Starting point of Routes
    strcpy(date, "05/16/2024");
    strcpy(origin, "Legazpi");
    float discountRate = 18.45; // Discount Info - discount rate is in percent

    Passenger passengerInfo; // passenger information of a booking
    Operator loggedOperator;

    Summary passengerSummary;
    // initializes data to 0
    passengerSummary.ticketsNumTotal = 0;
    passengerSummary.regularTicketsTotal = 0;
    passengerSummary.discountTicketsTotal = 0;
    passengerSummary.totalSales = 0.0;

    // Initializes multidimensional array to 0
    for (int i = 0; i < numOfRoutes; i++)
    {
        for (int j = 0; j < numOfPrice; j++)
        {
            passengerSummary.ticketCountRegular[i][j] = 0;
            passengerSummary.ticketCountDiscount[i][j] = 0;
        }
    }

    do
    {
        // main menu
        printf("=========================================================================\n\t\t\tBus Ticketing System \n=========================================================================\n");
        printf(" 1. Passenger\n");
        printf(" 2. Operator \n");
        printf(" 3. Exit     \n");
        printf("-----------------------------------\n");
        printf("Please enter the number corresponding to your choice: ");
        scanf(" %c", &mainOption);

        switch (mainOption)
        {
        case '1': // if passenger
            printf("\n=========================================================================\n\t\t\tPassenger Dashboard \n=========================================================================\n");
            printf("Welcome Passenger!\n\n");
            printf("Main Menu:\n 1. Book Ticket\n 2. FAQs\n 3. Exit\n");
            printf("-----------------------------------\n");
            printf("Please enter the number corresponding to your choice: ");
            scanf(" %c", &bookingOption);

            switch (bookingOption)
            {
            case '1': // Book Ticket
                if (numOfBookings >= MAX_PASSENGERS)
                {
                    printf("Maximum number of passengers is reached.\n");
                }
                else
                {
                    searchResult = 0;
                    printf("\n=========================================================================\n\t\t\tBook Ticket \n=========================================================================\n\n");
                    printf("Starting Point: %s\n", origin);
                    printf("Available Routes: \n");
                    displayRoute(routeList, numOfRoutes);
                    printf("\n-----------------------------------\n");
                    printf("Please enter the following details to book a bus ticket:\n");
                    printf("Route ID: ");
                    scanf("%d", &routeIdChoice);
                    searchResult = findDestinationByRouteId(routeList, numOfRoutes, routeIdChoice, passengerInfo.destination);
                    if (searchResult == 0)
                    {
                        printf("Route is not available at the moment.\n");
                        continue;
                    }
                    else if (searchResult == -1)
                    {
                        printf("Route ID not found.\n");
                        continue;
                    }
                    else
                    {
                        do
                        {
                            printf("Name (Put \"-\" between First name and Last Name): ");
                            scanf("%s", passengerInfo.name);
                            printf("Select Bus Type: \n");
                            for (int i = 0; i < numOfPrice; i++)
                            {
                                printf(" %d. %s\n", priceList[i].priceRef, priceList[i].busType);
                            }
                            printf("Please enter the number corresponding to your choice: ");
                            scanf("%d", &busTypeChoice);

                            if (busTypeChoice >= 1 && busTypeChoice <= numOfPrice)
                            {
                                strcpy(passengerInfo.busType, priceList[busTypeChoice - 1].busType);
                            }
                            else
                            {
                                printf("Invalid choice. Try Again\n");
                            }
                            printf("Number of Tickets: ");
                            scanf("%d", &passengerInfo.ticketsNum);
                            printf("   Number of Discounted Passengers(Put 0 if none): ");
                            scanf("%d", &passengerInfo.discountTickets);
                            passengerInfo.regTickets = passengerInfo.ticketsNum - passengerInfo.discountTickets;
                            // Calculate payment
                            calculatePayment(&passengerInfo, priceList, routeList, discountRate, routeIdChoice, numOfRoutes, numOfPrice);

                            // User Check in
                            printf("\n------------Ticket Summary--------------\n");
                            printf("Name:            %s\n", passengerInfo.name);
                            printf("Destination:     %s\n", passengerInfo.destination);
                            printf("Bus Type:        %s\n", passengerInfo.busType);
                            printf("----------------------------------------\n");
                            printf("Ticket Price:    ₱%.2lf\n", passengerInfo.ticketPrice);
                            printf("Discount Rate:   %.2f%%\n", discountRate);
                            printf("----------------------------------------\n");
                            printf("Passengers:      %d\n", passengerInfo.ticketsNum);
                            printf("  Regular:       %d\n", passengerInfo.regTickets);
                            printf("  Discounted:    %d\n", passengerInfo.discountTickets);
                            printf("----------------------------------------\n");
                            printf("Total Payment:   ₱%.2lf\n", passengerInfo.totalPayment);
                            printf("----------------------------------------\n\n");

                            printf("Main Menu:\n 1. Confirm Booking\n 2. Cancel Booking\n");
                            printf("-----------------------------------\n");
                            printf("Please enter the number corresponding to your choice: ");
                            scanf(" %c", &printOption);

                            switch (printOption)
                            {
                            case '1': // Confirm Booking
                                printf("Booking Confirmed.\n");
                                printTicket(&passengerInfo, ticketFile, routeIdChoice, origin, date);
                                // Addition of Bookings in the passengerList
                                addTicket(&numOfBookings, bookingList, &passengerInfo);
                                // Print to booking input file
                                updateBookings(bookingUpdateFile, bookingOutputFile, numOfBookings, bookingList);
                                fclose(bookingUpdateFile);
                                fclose(bookingOutputFile);
                                break;
                            case '2': // Cancel Booking
                                printf("Booking Cancelled.\n");
                                break;
                            default:
                                printf("Invalid choice. Try Again\n");
                                continue;
                            }
                            break;
                        } while (printOption != '2');
                    }
                }
                break;
            case '2': // FAQS
                displayFAQs();
                continue;
            case '3': // EXIT
                break;
            default:
                printf("Invalid choice. Try Again\n");
            }
            break;

        case '2': // OPERATOR SIDE
            do
            {
                printf("\n=========================================================================\n\t\tOperator Login \n=========================================================================\n");
                printf(" Enter Employee ID: ");
                scanf("%ld", &loggedOperator.idNumber);
                printf(" Enter 5-digit PIN: ");
                scanf("%d", &loggedOperator.pin);
                // Search operator
                passResult = 0;
                passResult = searchOperator(&loggedOperator, operatorList, numOfOperator);
                if (passResult == 1)
                {
                    do
                    {
                        printf("\n=========================================================================\n\t\tOperator Dashboard \n=========================================================================\n");
                        printf("Welcome back, %s %s\n\n", loggedOperator.firstName, loggedOperator.lastName);
                        printf("Main Menu:\n 1. Routes \n 2. Prices and Discounts\n 3. Passenger List\n 4. Account Information\n 5. Logout\n");
                        printf("-----------------------------------\n");
                        printf("Please enter the number corresponding to your choice: ");
                        scanf(" %c", &operatorOption);

                        switch (operatorOption)
                        {
                        case '1': // Routes
                            do
                            {
                                printf("\n=========================================================================\n\t\tRoutes\n=========================================================================\n\n");
                                printf("Date: %s\n", date);
                                printf("Starting Point: %s\n", origin);
                                printf("Available Routes: \n");
                                displayOperatorRoute(routeList, numOfRoutes);
                                printf("-----------------------------------\n");
                                printf("Main Menu:\n 1. Add Route\n 2. Edit Route \n 3. Delete Route \n 4. Change Starting Point \n 5. Change Date \n 6. Back to dashboard\n");
                                printf("-----------------------------------\n");
                                printf("Please enter the number corresponding to your choice: ");
                                scanf(" %c", &routeOption);
                                switch (routeOption)
                                {
                                case '1': // Add Route
                                    numOfRoutes++;
                                    if (numOfRoutes >= MAX_ROUTE)
                                    {
                                        printf("Maximum number of routes is reached.\n");
                                    }
                                    else
                                    {
                                        printf("\n=========================================================================\n\t\tAdd Route \n=========================================================================\n\n");
                                        routeList[numOfRoutes - 1].routeRef = numOfRoutes;
                                        printf("Route Destination (Put '-' between words): ");
                                        scanf("%s", routeList[numOfRoutes - 1].destination);
                                        printf("Route Distance from Origin: ");
                                        scanf("%f", &(routeList[numOfRoutes - 1].distanceKm));
                                        printf("Route Status (Available/Not-Available): ");
                                        scanf("%s", routeList[numOfRoutes - 1].status);
                                        if (strcmp(routeList[numOfRoutes - 1].status, "Available") == 0 || strcmp(routeList[numOfRoutes - 1].status, "Not-Available") == 0)
                                        {
                                            printf("-----------------------------------\n");
                                            printf("Route is added.\n");
                                        }
                                        else
                                        {
                                            strcpy(routeList[numOfRoutes - 1].status, "Not-Specified");
                                            printf("Invalid Status. Must be \"Available\" or \"Not-Available\"");
                                            continue;
                                        }
                                    }
                                    break;
                                case '2': // Edit Route
                                    printf("\n=========================================================================\n\t\tEdit Existing Route \n=========================================================================\n\n");
                                    displayOperatorRoute(routeList, numOfRoutes);
                                    printf("Select Route to edit:\nRoute ID: ");
                                    scanf("%d", &routeIdChoice);
                                    printf("\n===================================\n");
                                    printf("Destination: %s\n", routeList[routeIdChoice - 1].destination);
                                    printf("Select part to edit:\n 1. Distance from Origin\n 2. Status \n 3. Back to Menu\n");
                                    printf("-----------------------------------\n");
                                    printf("Please enter the number corresponding to your choice: ");
                                    scanf(" %c", &editOption);

                                    switch (editOption)
                                    {
                                    case '1': // Edit Distance
                                        printf("Current Distance from Origin: %f\n", routeList[routeIdChoice - 1].distanceKm);
                                        printf("New Distance (km): ");
                                        scanf("%f", &routeList[routeIdChoice - 1].distanceKm);
                                        printf("-----------------------------------\n");
                                        printf("\nRoute Distance is changed.\n");
                                        break;
                                    case '2': // Edit Status
                                        printf("Current Status: %s\n", routeList[routeIdChoice - 1].status);
                                        printf("New Status (Available/Not-Available): ");
                                        scanf("%s", routeList[routeIdChoice - 1].status);
                                        if (strcmp(routeList[routeIdChoice - 1].status, "Available") == 0 || strcmp(routeList[routeIdChoice - 1].status, "Not-Available") == 0)
                                        {
                                            printf("-----------------------------------\n");
                                            printf("\nRoute Status is changed.\n");
                                        }
                                        else
                                        {
                                            strcpy(routeList[routeIdChoice - 1].status, "Not-Specified");
                                            printf("Invalid Status. Must be \"Available\" or \"Not-Available\"");
                                            continue;
                                        }

                                        break;
                                    case '3': // Back to menu
                                        break;
                                    default:
                                        printf("Invalid input. Try Again\n");
                                    }
                                    break;
                                case '3': // Delete Route -> SOLUTION: swap struct to delete wth the last struct in array then delete struct
                                    printf("\n=========================================================================\n\t\tDelete Route \n=========================================================================\n\n");
                                    displayOperatorRoute(routeList, numOfRoutes);
                                    printf("Select Route to delete:\nRoute ID: ");
                                    scanf("%d", &routeIdChoice);
                                    // function to swap struct to delete with last struct
                                    // 1. Search array with same route num
                                    searchResult = 0;
                                    searchResult = searchRoute(routeList, routeIdChoice, numOfRoutes);
                                    if (searchResult == 1)
                                    {
                                        swapRoute(&routeList[routeIdChoice - 1], &routeList[numOfRoutes - 1]);
                                        numOfRoutes--;
                                        printf("-----------------------------------\n");
                                        printf("\nRoute is deleted.\n");
                                    }
                                    else
                                    {
                                        printf("Route ID not found");
                                    }
                                    break;
                                case '4': // Change Origin/Starting
                                    printf("\n=========================================================================\n\t\tChange Starting Point \n=========================================================================\n\n");
                                    printf("Current Starting Point: %s\n", origin);
                                    printf("New Starting Point (Put '-' between words): ");
                                    scanf("%s", origin);
                                    printf("-----------------------------------\n");
                                    printf("\nStarting point is changed.\n");
                                    break;
                                case '5': // Change Date
                                    printf("\n=========================================================================\n\t\tChange Date \n=========================================================================\n\n");
                                    printf("Current Date: %s\n", date);
                                    printf("New Date(MM/DD/YYYY): ");
                                    scanf("%s", date);
                                    printf("-----------------------------------\n");
                                    printf("\nDate is changed.\n");
                                    break;
                                case '6': // Exit
                                    break;
                                default:
                                    printf("Invalid choice. Try again\n");
                                }
                                break;
                            } while (routeOption != '6');
                            break;
                        case '2': // Prices and Discounts
                            do
                            {
                                printf("\n=========================================================================\n\t\tPrices and Discounts\n=========================================================================\n\n");
                                printf("Discount Rate: %.2f%%\n", discountRate);
                                printf("Available Category and Rates: \n");
                                displayPrice(priceList, numOfPrice);
                                printf("-----------------------------------\n");
                                printf("Main Menu:\n 1. Change Discount Rate\n 2. Add Category\n 3. Edit Category\n 4. Delete Category\n 5. Back to dashboard\n");
                                printf("-----------------------------------\n");
                                printf("Please enter the number corresponding to your choice: ");
                                scanf(" %c", &priceOption);

                                switch (priceOption)
                                {
                                case '1': // Discount Rate
                                    printf("\n=========================================================================\n\t\tChange Discount\n=========================================================================\n\n");
                                    printf("Current Discount Rate: %.2f%%\n", discountRate);
                                    printf("New Discount Rate (in %%): ");
                                    scanf("%f", &discountRate);
                                    printf("-----------------------------------\n");
                                    printf("\nDiscount Rate is changed.\n");
                                    break;
                                case '2': // Add Category
                                    numOfPrice++;
                                    if (numOfPrice >= MAX_CATEGORY)
                                    {
                                        printf("Maximum number of category is reached.\n");
                                    }
                                    else
                                    {
                                        printf("\n=========================================================================\n\t\tAdd Category \n=========================================================================\n\n");
                                        priceList[numOfPrice - 1].priceRef = numOfPrice;
                                        printf("Price Category (Put '-' between words): ");
                                        scanf("%s", priceList[numOfPrice - 1].busType);
                                        printf("Price Rate (Php/km): ");
                                        scanf("%f", &(priceList[numOfPrice - 1].pricePhpKm));
                                        printf("-----------------------------------\n");
                                        printf("Price Category is added.\n");
                                    }
                                    break;
                                case '3': // Edit Category
                                    printf("\n=========================================================================\n\t\tEdit Existing Category\n=========================================================================\n\n");
                                    displayPrice(priceList, numOfPrice);
                                    printf("Select Category to edit:\nPrice ID: ");
                                    scanf("%d", &priceIdChoice);
                                    printf("\n===================================\n");
                                    printf("Category: %s\n", priceList[priceIdChoice - 1].busType);
                                    printf("Current Price Rate (Php/km): %.2f\n", priceList[priceIdChoice - 1].pricePhpKm);
                                    printf("New Price Rate (Php/km): ");
                                    scanf("%f", &priceList[priceIdChoice - 1].pricePhpKm);
                                    printf("-----------------------------------\n");
                                    printf("Price Category is changed.\n");
                                    break;
                                case '4': // Delete Category
                                    printf("\n=========================================================================\n\t\tDelete Category \n=========================================================================\n\n");
                                    displayPrice(priceList, numOfPrice);
                                    printf("Select Category to delete:\nPrice ID: ");
                                    scanf("%d", &priceIdChoice);
                                    // function to swap struct to delete with last struct
                                    // 1. Search array with same route num
                                    searchResult = 0;
                                    searchResult = searchCategory(priceList, priceIdChoice, numOfPrice);
                                    if (searchResult == 1)
                                    {
                                        swapCategory(&priceList[priceIdChoice - 1], &priceList[numOfPrice - 1]);
                                        numOfPrice--;
                                        printf("-----------------------------------\n");
                                        printf("Category is deleted.\n");
                                    }
                                    else
                                    {
                                        printf("Route ID not found.");
                                    }
                                    break;
                                case '5': // EXIT
                                    break;
                                default:
                                    printf("Invalid choice. Try again\n");
                                }
                                break;
                            } while (priceOption != '5');
                            break;
                        case '3': // Passenger List
                            printf("\n=========================================================================\n\t\tPassenger List\n=========================================================================\n\n");
                            printf("==============================================================================================================================================================\n");
                            printf("| %-9s | %-18s | %-18s | %-18s | %-19s | %-15s | %-8s | %-11s | %-14s |\n", "Ref. No.", "Name", "Route-Destination", "Bus Type", "Ticket Price (PHP)", "No. of Tickets", "Regular", "Discounted", "Total Payment");
                            printf("==============================================================================================================================================================\n");
                            for (int i = 0; i < numOfBookings; i++)
                            {
                                printf("| %-9d | %-18s | %-18s | %-18s | %-19.2f | %-15d | %-8d | %-11d | %-14.2lf |\n", bookingList[i].passengerID, bookingList[i].name, bookingList[i].destination, bookingList[i].busType, bookingList[i].ticketPrice, bookingList[i].ticketsNum, bookingList[i].regTickets, bookingList[i].discountTickets, bookingList[i].totalPayment);
                                summarizePassengerList(&bookingList[i], &passengerSummary);
                                countTicketsPerRoute(&bookingList[i], &passengerSummary, routeList, priceList, numOfRoutes, numOfPrice);
                            }
                            printf("==============================================================================================================================================================\n");
                            // Add a summary part at the end:
                            printf("SUMMARY: \n\n");
                            printf("Date:                                        %s\n", date);
                            printf("TOTAL NUMBER OF PASSENGERS:                  %ld\n", numOfBookings);
                            printf("Number of Tickets Sold:                      %ld\n", passengerSummary.ticketsNumTotal);
                            printf("  Regular Tickets:                           %ld\n", passengerSummary.regularTicketsTotal);
                            printf("  Discounted Tickets:                        %ld\n", passengerSummary.discountTicketsTotal);
                            printf("  Total Sales:                               %.2lf\n", passengerSummary.totalSales);
                            for (int i = 0; i < numOfRoutes; i++)
                            {
                                printf("==================================================\n");
                                printf("--%s--\n", routeList[i].destination);
                                for (int j = 0; j < numOfPrice; j++)
                                {
                                    printf("--------------------------------------------------\n");
                                    printf("    -%s:\n", priceList[j].busType);
                                    printf("      Regular Tickets:                      %ld\n", passengerSummary.ticketCountRegular[i][j]);
                                    printf("      Discounted Tickets:                   %ld\n", passengerSummary.ticketCountDiscount[i][j]);
                                }
                            }
                            printf("=====================================================\n");
                            char printSummaryChoice;
                            printf("Print Summary? (1 for YES, 0 for NO): ");
                            scanf(" %c", &printSummaryChoice);
                            if (printSummaryChoice == '1')
                            {
                                printSummary(summaryFile, &passengerSummary, numOfRoutes, numOfPrice, date, numOfBookings, routeList, priceList);
                                fclose(summaryFile);
                                printf("Summary is printed (Please check summary.txt)\n");
                            }
                            else if (printSummaryChoice != '0')
                            {
                                printf("Invalid input.\n");
                                continue;
                            }
                            break;
                        case '4': // Account Information
                            printf("\n===================================================================================================\n\t\t\tAccount Information\n===================================================================================================\n\n");
                            // Print the following:
                            // Operator ID:
                            printf("Operator ID: %ld\n", loggedOperator.idNumber);
                            // Operator Name:
                            printf("Name: %s %s\n", loggedOperator.firstName, loggedOperator.lastName);
                            // Operator PIN:
                            printf("PIN: %d\n", loggedOperator.pin);
                            // Instruction on how to use program:
                            displayInstruction();
                            break;
                        case '5': // Log out
                            break;
                        default:
                            printf("Invalid choice. Try again\n");
                        }
                    } while (operatorOption != '5');
                    break;
                }
                else if (passResult == 2)
                {
                    printf("Incorrect PIN. Try Again\n");
                    break;
                }
                else
                {
                    printf("Employee ID not found.\n");
                    break;
                }
            } while (operatorOption != '5');
            break;

        case '3': // EXIT
            updateRoute(routeUpdateFile, numOfRoutes, routeList);
            fclose(routeUpdateFile);
            updatePrice(priceUpdateFile, numOfPrice, priceList);
            fclose(priceUpdateFile);
            break;

        default:
            printf("Invalid choice. Try again\n");
        }

    } while (mainOption != '3');

    return 0;
}

// Function to print a ticket for a passenger
void printTicket(Passenger *passengerInfo, FILE *ticketFile, int routeId, char *origin, char *date)
{
    // Print ticket header
    fprintf(ticketFile, "==================================================================\n\t\t\tBUS TICKET\n==================================================================\n");
    // Print passenger name and date
    fprintf(ticketFile, "Name:                      %s\n", passengerInfo->name);
    fprintf(ticketFile, "Date:                      %s\n", date);
    fprintf(ticketFile, "------------------------------------------------------------------\n");
    // Print route details
    fprintf(ticketFile, "Route Details: \n");
    fprintf(ticketFile, "Route:                     #%d: %s-%s\n", routeId, origin, passengerInfo->destination);
    fprintf(ticketFile, "Bus Type:                  %s\n", passengerInfo->busType);
    fprintf(ticketFile, "Ticket Price:              ₱%.2f\n", passengerInfo->ticketPrice);
    fprintf(ticketFile, "------------------------------------------------------------------\n");
    // Print ticket summary
    fprintf(ticketFile, "Ticket Summary: \n");
    fprintf(ticketFile, "Number of Tickets:         %d\n", passengerInfo->ticketsNum);
    fprintf(ticketFile, "Regular Tickets:           %d\n", passengerInfo->regTickets);
    fprintf(ticketFile, "Discount Tickets:          %d\n", passengerInfo->discountTickets);
    fprintf(ticketFile, "------------------------------------------------------------------\n");
    // Print total price
    fprintf(ticketFile, "Total Price:               ₱%.2lf\n", passengerInfo->totalPayment);
    // Print footer
    fprintf(ticketFile, "==================================================================\n");
    fprintf(ticketFile, "Thank you for choosing our bus service!\nSafe travels!\n");
    fprintf(ticketFile, "==================================================================\n");
    fclose(ticketFile);
}

// Function to search for an operator by ID and PIN
int searchOperator(Operator *operatorCheck, Operator *operatorList, int numOfOperator)
{
    int flag = 0;
    for (int i = 0; i < numOfOperator; i++)
    {
        if (operatorCheck->idNumber == operatorList[i].idNumber)
        {
            if (operatorCheck->pin == operatorList[i].pin)
            {
                // Operator found, return flag 1
                strcpy(operatorCheck->firstName, operatorList[i].firstName);
                strcpy(operatorCheck->lastName, operatorList[i].lastName);
                flag = 1;
                return flag;
            }
            else
            {
                // ID matched but PIN didn't, return flag 2
                flag = 2;
                return flag;
            }
        }
    }
    // No match found, return flag 0
    return flag;
}

// Function to search for a route by ID
int searchRoute(Route *routeList, int routeIdCheck, int numOfRoutes)
{
    int flag = 0;
    for (int i = 0; i < numOfRoutes; i++)
    {
        if (routeIdCheck == routeList[i].routeRef)
        {
            // Route found, return flag 1
            flag = 1;
            return flag;
        }
    }
    // No match found, return flag 0
    return flag;
}

// Function to search for a category by ID
int searchCategory(Price *priceList, int priceIdCheck, int numOfPrice)
{
    int flag = 0;
    for (int i = 0; i < numOfPrice; i++)
    {
        if (priceIdCheck == priceList[i].priceRef)
        {
            // Category found, return flag 1
            flag = 1;
            return flag;
        }
    }
    // No match found, return flag 0
    return flag;
}

// Function to swap two route structures
void swapRoute(Route *route1, Route *route2)
{
    Route tempRoute;
    // Swap destination, distance, and status between routes
    strcpy(tempRoute.destination, route1->destination);
    tempRoute.distanceKm = route1->distanceKm;
    strcpy(tempRoute.status, route1->status);

    strcpy(route1->destination, route2->destination);
    route1->distanceKm = route2->distanceKm;
    strcpy(route1->status, route2->status);

    strcpy(route2->destination, tempRoute.destination);
    route2->distanceKm = tempRoute.distanceKm;
    strcpy(route2->status, tempRoute.status);
}

// Function to swap two price structures
void swapCategory(Price *price1, Price *price2)
{
    Price tempPrice;
    // Swap busType and rate between categories
    strcpy(tempPrice.busType, price1->busType);
    tempPrice.pricePhpKm = price1->pricePhpKm;

    strcpy(price1->busType, price2->busType);
    price1->pricePhpKm = price2->pricePhpKm;

    strcpy(price2->busType, tempPrice.busType);
    price2->pricePhpKm = tempPrice.pricePhpKm;
}

// Function to summarize passenger information
void summarizePassengerList(Passenger *passenger, Summary *summaryPassenger)
{
    // Update summary with passenger data
    summaryPassenger->ticketsNumTotal += passenger->ticketsNum;
    summaryPassenger->regularTicketsTotal += passenger->regTickets;
    summaryPassenger->discountTicketsTotal += passenger->discountTickets;
    summaryPassenger->totalSales += passenger->totalPayment;
}

// Function to count tickets per route and category
void countTicketsPerRoute(Passenger *passenger, Summary *summaryPassenger, Route *routeList, Price *priceList, int numOfRoutes, int numOfPrice)
{
    // Iterate over routes and categories
    for (int j = 0; j < numOfRoutes; j++)
    {
        if (strcmp(passenger->destination, routeList[j].destination) == 0)
        {
            // Destination matches
            for (int k = 0; k < numOfPrice; k++)
            {
                if (strcmp(passenger->busType, priceList[k].busType) == 0)
                {
                    // Bus type matches
                    // Update ticket counts
                    summaryPassenger->ticketCountRegular[j][k] += passenger->regTickets;
                    summaryPassenger->ticketCountDiscount[j][k] += passenger->discountTickets;
                    break;
                }
            }
        }
    }
}

// Function to print a summary of ticket sales
void printSummary(FILE *summaryFile, Summary *summaryPassenger, int numOfRoutes, int numOfPrice, char *date, int numOfPassenger, Route *routeList, Price *priceList)
{
    // Print summary header
    fprintf(summaryFile, "==================================================================\n\t\t\tTICKET SUMMARY\n==================================================================\n");
    // Print date and total passengers
    fprintf(summaryFile, "Date:                                %s\n", date);
    fprintf(summaryFile, "TOTAL NUMBER OF PASSENGERS:          %d\n", numOfPassenger);
    // Print total number of tickets sold
    fprintf(summaryFile, "Number of Tickets Sold:              %ld\n", summaryPassenger->ticketsNumTotal);
    fprintf(summaryFile, "==================================================================\n");
    // Print regular and discounted tickets
    fprintf(summaryFile, "  Regular Tickets:                   %ld\n", summaryPassenger->regularTicketsTotal);
    fprintf(summaryFile, "  Discounted Tickets:                %ld\n", summaryPassenger->discountTicketsTotal);
    fprintf(summaryFile, "==================================================================\n");
    // Print total sales
    fprintf(summaryFile, "  Total Sales:                       %.2lf\n", summaryPassenger->totalSales);
    // Print ticket counts per route and category
    for (int i = 0; i < numOfRoutes; i++)
    {
        fprintf(summaryFile, "==================================================================\n");
        fprintf(summaryFile, "--%s--\n", routeList[i].destination);
        for (int j = 0; j < numOfPrice; j++)
        {
            fprintf(summaryFile, "==================================================================\n");
            fprintf(summaryFile, "    -%s:\n", priceList[j].busType);
            fprintf(summaryFile, "      Regular Tickets:       %ld\n", summaryPassenger->ticketCountRegular[i][j]);
            fprintf(summaryFile, "      Discounted Tickets:    %ld\n", summaryPassenger->ticketCountDiscount[i][j]);
        }
    }
    fprintf(summaryFile, "==================================================================\n");
}

// Function to calculate payment for a passenger
void calculatePayment(Passenger *passengerInfo, Price *priceList, Route *routeList, float discountRate, int routeIdChoice, int numOfRoutes, int numOfPrice)
{
    int routeFlag = 0, priceFlag = 0;

    // Calculate ticket price based on route and category
    for (int i = 0; i < numOfRoutes; i++)
    {
        if (routeList[i].routeRef == routeIdChoice)
        {
            // Route found
            routeFlag = 1;
            for (int j = 0; j < numOfPrice; j++)
            {
                if (strcmp(passengerInfo->busType, priceList[j].busType) == 0)
                {
                    // Category found
                    priceFlag = 1;
                    // Calculate ticket price
                    passengerInfo->ticketPrice = routeList[i].distanceKm * priceList[j].pricePhpKm;
                    break;
                }
            }
            break;
        }
    }

    // Check if route and category were found
    if (!routeFlag)
    {
        printf("Route not found.\n");
        return; // Return early if route is not found
    }

    if (!priceFlag)
    {
        printf("Category not found.\n");
        return; // Return early if category is not found
    }

    // Calculate total payment including discounts
    passengerInfo->totalPayment = (passengerInfo->ticketPrice * passengerInfo->regTickets) + ((passengerInfo->ticketPrice - (passengerInfo->ticketPrice * (discountRate / 100.00))) * passengerInfo->discountTickets);
}

// Function to find the destination by route ID
int findDestinationByRouteId(Route *routeList, int numOfRoutes, int routeIdChoice, char *passengerDestination)
{
    for (int i = 0; i < numOfRoutes; i++)
    {
        if (routeList[i].routeRef == routeIdChoice)
        {
            if (strcmp(routeList[i].status, "Not-Available") == 0 || strcmp(routeList[i].status, "Not-Specified") == 0)
            {
                return 0; // Route not available
            }
            else
            {
                strcpy(passengerDestination, routeList[i].destination);
                return 1; // Found and destination set
            }
        }
    }
    return -1; // Route ID not found
}

// Function to display frequently asked questions
void displayFAQs()
{
    // Print FAQ section
    printf("\n=========================================================================\n\t\t\tFAQs\n=========================================================================\n\n");

    printf("1. How do I book a ticket?\n");
    printf("   To book a ticket, go to the Passenger Dashboard (Option 1 from the main menu)\n   Follow the prompts to enter your details, choose your route, bus type, and \n   number of passengers. Confirm to finalize.\n\n");

    printf("2. Can I view available routes before booking?\n");
    printf("   Yes, you can view available routes on the Passenger Dashboard.\n   Select Option 1 and choose your destination.\n\n");

    printf("3. What information do I need to provide when booking?\n");
    printf("   Provide your name, route ID, preferred bus type,\n   and number of passengers.\n\n");

    printf("4. How do I confirm my booking?\n");
    printf("   After entering your details, review the summary \n   and confirm your booking when prompted.\n\n");

    printf("5. Is there a limit to the number of passengers I can book for?\n");
    printf("   Yes, there's a maximum limit per trip. \n   You'll be notified if the limit is reached.\n\n");

    printf("6. Can I cancel my booking after confirming?\n");
    printf("   No, bookings cannot be canceled through the system. \n   Contact customer service for inquiries.\n\n");

    printf("7. How do I pay for my ticket?\n");
    printf("   Payment is made at the terminal cashier after confirming your booking.\n\n");

    printf("8. Are there any discounts available?\n");
    printf("   Currently, we don't offer discounts online. \n   Stay updated for announcements.\n\n");

    printf("=========================================================================\n\t\t\tPassenger Instructions\n=========================================================================\n\n");

    printf("1. Passenger Dashboard:\n");
    printf("   - To book, select Option 1 and follow instructions.\n");
    printf("   - Choose route, bus type, and number of passengers.\n\n");

    printf("2. Booking a Ticket:\n");
    printf("   - Enter your details and confirm.\n\n");

    printf("=========================================================================\n\t\t\tContact Information\n=========================================================================\n\n");

    printf("For inquiries or assistance:\n");

    printf("Phone: 099449933333\n");
    printf("Email: busSys@gmail.com\n");
    printf("Website: www.busSys.com.ph\n\n");

    printf("Our customer service team is available 8:00 AM - 9:00 PM.\n");
    printf("Thank you for choosing SysBus. We look forward to serving you.\n");
}

// Function to read operator data from a file
void readOperatorFile(FILE *operatorFile, Operator *operatorList, int numOfOperator)
{
    // Read operator data from file
    for (int i = 0; i < numOfOperator; i++)
    {
        fscanf(operatorFile, " %ld %s %s %d", &operatorList[i].idNumber, operatorList[i].firstName, operatorList[i].lastName, &operatorList[i].pin);
    }
    fclose(operatorFile);
}

// Function to read route data from a file
void readRouteFile(FILE *routeFile, Route *routeList, int numOfRoutes)
{
    // Read route data from file
    for (int i = 0; i < numOfRoutes; i++)
    {
        fscanf(routeFile, "%d %s %f %s", &routeList[i].routeRef, routeList[i].destination, &routeList[i].distanceKm, routeList[i].status);
    }
    fclose(routeFile);
}

// Function to read price data from a file
void readPriceFile(FILE *pricingFile, Price *priceList, int numOfPrice)
{
    // Read price data from file
    for (int i = 0; i < numOfPrice; i++)
    {
        fscanf(pricingFile, "%d %s %f", &priceList[i].priceRef, priceList[i].busType, &priceList[i].pricePhpKm);
    }
    fclose(pricingFile);
}

// Function to read booking input from a file
void readBookingInput(FILE *bookingInputFile, Passenger *bookingList, long int numOfBookings)
{
    // Read booking input from file
    if (numOfBookings > 0)
    {
        for (int i = 0; i < numOfBookings; i++)
            fscanf(bookingInputFile, "%d %s %s %s %f %d %d %d %lf", &bookingList[i].passengerID, bookingList[i].name, bookingList[i].destination, bookingList[i].busType, &bookingList[i].ticketPrice, &bookingList[i].ticketsNum, &bookingList[i].regTickets, &bookingList[i].discountTickets, &bookingList[i].totalPayment);
    }
}

// Function to add a ticket to the booking list and update input and output files
void addTicket(long int *numOfBookings, Passenger *bookingList, Passenger *passengerInfo) {
    // Assign passenger information to booking list
    bookingList[*numOfBookings].passengerID = (*numOfBookings) + 1;
    strcpy(bookingList[*numOfBookings].name, passengerInfo->name);
    strcpy(bookingList[*numOfBookings].destination, passengerInfo->destination);
    strcpy(bookingList[*numOfBookings].busType, passengerInfo->busType);
    bookingList[*numOfBookings].ticketPrice = passengerInfo->ticketPrice;
    bookingList[*numOfBookings].ticketsNum = passengerInfo->ticketsNum;
    bookingList[*numOfBookings].regTickets = passengerInfo->regTickets;
    bookingList[*numOfBookings].discountTickets = passengerInfo->discountTickets;
    bookingList[*numOfBookings].totalPayment = passengerInfo->totalPayment;

    // Increment booking list size
    (*numOfBookings)++;
}



// Function to display route details
void displayRoute(Route *routeList, int numOfRoutes)
{
    printf("| %-8s | %-15s | %-15s |\n", "ROUTE ID", "DESTINATION", "STATUS");
    for (int i = 0; i < numOfRoutes; i++)
    {
        printf("| %-8d | %-15s | %-15s |\n", routeList[i].routeRef, routeList[i].destination, routeList[i].status);
    }
}

// Function to display route details including distance
void displayOperatorRoute(Route *routeList, int numOfRoutes)
{
    printf("| %-8s | %-15s | %-15s | %-15s |\n", "ROUTE ID", "DESTINATION", "DISTANCE (km)", "STATUS");
    for (int i = 0; i < numOfRoutes; i++)
    {
        printf("| %-8d | %-15s | %-15.2f | %-15s |\n", routeList[i].routeRef, routeList[i].destination, routeList[i].distanceKm, routeList[i].status);
    }
}

// Function to display price details
void displayPrice(Price *priceList, int numOfPrice)
{
    printf("| %-8s | %-15s | %-15s |\n", "PRICE ID", "CATEGORY", "RATE(₱/km)");
    for (int i = 0; i < numOfPrice; i++)
        printf("| %-8d | %-15s | %-14.2f|\n", priceList[i].priceRef, priceList[i].busType, priceList[i].pricePhpKm);
}

// Function to display operator instructions
void displayInstruction()
{
    printf("\n=========================================================================\n\t\t\tOperator Instructions\n=========================================================================\n\n");

    printf("1. Accessing the Operator Dashboard:\n");
    printf("   - Use your employee ID number and PIN to log in.\n");
    printf("   - Once logged in, you'll have control over routes, prices,\n   discounts, and passenger lists.\n\n");

    printf("2. Managing Routes:\n");
    printf("   - Add, edit, or delete routes as needed.\n");
    printf("   - Adjust starting points or dates to reflect schedule changes.\n\n");

    printf("3. Managing Prices and Discounts:\n");
    printf("   - Modify discount rates and bus category prices.\n");
    printf("   - Ensure accurate fare calculations and update pricing policies.\n\n");

    printf("4. Viewing Passenger Lists:\n");
    printf("   - Access passenger lists to monitor bookings and seat availability.\n");
    printf("   - Track passenger numbers and view ticket details.\n\n");

    printf("5. Accessing Account Information:\n");
    printf("   - View your employee ID, name, and PIN from the Operator Dashboard.\n");
    printf("   - Ensure secure access to the system.\n\n");

    printf("6. Logging Out:\n");
    printf("   - Remember to log out after completing tasks to protect\n   sensitive information.\n");
}

// Function to update price details in the price file
void updatePrice(FILE *priceFile, int numOfPrice, Price *priceList)
{
    fprintf(priceFile, "%d\n", numOfPrice);
    for (int i = 0; i < numOfPrice; i++)
    {
        fprintf(priceFile, "%-9d %-18s %-14.2f\n", priceList[i].priceRef, priceList[i].busType, priceList[i].pricePhpKm);
    }
}

// Function to update route details in the route file
void updateRoute(FILE *routeFile, int numOfRoute, Route *routeList)
{
    fprintf(routeFile, "%d\n", numOfRoute);
    for (int i = 0; i < numOfRoute; i++)
    {
        fprintf(routeFile, "%-9d %-18s %-14.2f %-18s\n", routeList[i].routeRef, routeList[i].destination, routeList[i].distanceKm, routeList[i].status);
    }
}

// Function to update route details in the route file
void updateBookings(FILE *bookingInputUpdateFile, FILE *bookingOutputFile, long int numOfBookings, Passenger *bookingList)
{
    fprintf(bookingInputUpdateFile, "%ld\n", numOfBookings);
    for (int i = 0; i < numOfBookings; i++)
    {
        fprintf(bookingInputUpdateFile, "%-9d %-18s %-18s %-20s %-19.2f %-15d %-8d %-11d %-14.2lf\n", bookingList[i].passengerID, bookingList[i].name, bookingList[i].destination, bookingList[i].busType, bookingList[i].ticketPrice, bookingList[i].ticketsNum, bookingList[i].regTickets, bookingList[i].discountTickets, bookingList[i].totalPayment);
        fprintf(bookingOutputFile, "| %-9d | %-18s | %-18s | %-18s | %-19.2f | %-15d | %-8d | %-11d | %-14.2lf |\n", bookingList[i].passengerID, bookingList[i].name, bookingList[i].destination, bookingList[i].busType, bookingList[i].ticketPrice, bookingList[i].ticketsNum, bookingList[i].regTickets, bookingList[i].discountTickets, bookingList[i].totalPayment);
    }
}
