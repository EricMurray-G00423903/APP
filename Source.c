#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Define player structure
typedef struct {
    int IRFU_number;
    char first_name[50];
    char last_name[50];
    int age;
    int height;
    int weight;
    char club[50];
    char email[50];
    char position[20];
    int tackles_missed;
    int meters_made;
    struct Player* next;
} PlayerDatabase;

// Function Prototypes
PlayerDatabase* createPlayerNode(int IRFU_number, char* first_name, char* last_name, int age, int height, int weight, char* club, char* email, char* position, int tackles_missed, int meters_made);
void readInDatabase(FILE* file);
void createNewDatabase(FILE* file);
void printHeader(int role);
void clearScreen();
void delayAndClear(int time, int role);
int authenticateUser(const char* username, const char* password);
void showMainMenu(int role);
void addPlayer();
void displayPlayerDetails(int role);
void displayAll(int role);
void editPlayer(int role);
void deletePlayer(int role);
void generateStats(int role);
void printReport(int role);
void exitProgram();
// End Prototypes

//GLOBAL LINKED LIST POINTERS
PlayerDatabase* head = NULL;
PlayerDatabase* tail = NULL;

// Function to create a new player node
PlayerDatabase* createPlayerNode(int IRFU_number, char* first_name, char* last_name, int age, int height, int weight, char* club, char* email, char* position, int tackles_missed, int meters_made) 
{
    PlayerDatabase* newNode = (PlayerDatabase*)malloc(sizeof(PlayerDatabase)); // Allocate memory for the new node
    if (newNode != NULL) {
        // Initialize the fields of the new node with the provided data
        newNode->IRFU_number = IRFU_number;
        strcpy(newNode->first_name, first_name);
        strcpy(newNode->last_name, last_name);
        newNode->age = age;
        newNode->height = height;
        newNode->weight = weight;
        strcpy(newNode->club, club);
        strcpy(newNode->email, email);
        strcpy(newNode->position, position);
        newNode->tackles_missed = tackles_missed;
        newNode->meters_made = meters_made;
        newNode->next = NULL; // Set the next pointer to NULL
    }
    return newNode;
}

// Function to read in File to Linked List
void readInDatabase(FILE* file) {
    int IRFU_number, age, height, weight, tackles_missed, meters_made;
    char first_name[50], last_name[50], club[50], email[50], position[20];
    char line[256]; // Buffer to hold each line from the file

    // Loop through each line in the file
    while (fgets(line, sizeof(line), file)) {
        int readItems = sscanf(line, "%d %49s %49s %d %d %d %49s %49s %19s %d %d", &IRFU_number, first_name, last_name, &age, &height, &weight, club, email, position, &tackles_missed, &meters_made);

        if (readItems != 11) {
            // If the line does not have 11 items, print an error and skip to the next line
            printf("DEBUG: Incorrect format in file. Skipping line.\n");
            continue;
        }

        // Create a new player node using the read data
        PlayerDatabase* newNode = createPlayerNode(IRFU_number, first_name, last_name, age, height, weight, club, email, position, tackles_missed, meters_made);

        if (newNode != NULL) {
            if (head == NULL) {
                // If the list is empty, set both head and tail to the new node
                head = newNode;
                tail = newNode;
            }
            else {
                // Otherwise, append the new node to the end of the list and update tail
                tail->next = newNode;
                tail = newNode;
            }
        }
        else {
            // Handle error: Failed to create node
            printf("Error: Failed to create player node\n");
        }
    }
}

// Function to create new database
void createNewDatabase(FILE* file) {

    printf("\nAre you sure you wish to continue? This will delete the previous database entirely!\n1) Yes\n2) No\n");
    int deleteDatabase = 0;
    
    while (deleteDatabase != 1 || deleteDatabase != 2)
    {
        printf("=> ");
        scanf("%d", &deleteDatabase);

        if (deleteDatabase == 2)
        {
            return;
        }

        if (deleteDatabase == 1)
        {
            break;
        }
    }

    // Close the file if it's already open
    if (file != NULL) {
        fclose(file);
    }

    // Delete the existing file
    if (remove("Rugby.txt") == 0) {
        printf("Previous database deleted successfully.\n");
    }
    else {
        printf("Error: Unable to delete the previous database. It might not exist.\n");
    }

    // Create a new file (or clear it if it already exists)
    file = fopen("Rugby.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to create a new database file.\n");
    }
    else {
        printf("A new database has been created successfully.\n\n");
        fclose(file); // Close the file after creation
    }
}

// Function to print the program header
void printHeader(int role) {
    char roleName[10];
    switch (role) {
    case 1:
        strcpy(roleName, "ADMIN ");
        break;
    case 2:
        strcpy(roleName, "USER ");
        break;
    case 3:
        strcpy(roleName, "GUEST");
        break;
    default:
        strcpy(roleName, "UNKNOWN");
        break;
    }

    printf("\x1b[42m"); // Set green background color
    printf("    ____ ____   ______ __  __   ____   __                             ____          __          __                    \n");
    printf("   /  _// __ \\ / ____// / / /  / __ \\ / /____ _ __  __ ___   _____   / __ \\ ____ _ / /_ ____ _ / /_   ____ _ _____ ___\n");
    printf("  / / / /_/ // /_   / / / /  / /_/ // // __ `// / / // _ \\ / ___/  / / / // __ `// __// __ `// __ \\ / __ `// ___// _ \\\n");
    printf(" /_/ / __  // __/  / /_/ /  / ____// // /_/ // /_/ //  __// /     / /_/ // /_/ // /_ / /_/ // /_/ // /_/ /(__  )/  __/\n");
    printf("/___//_/ |_|/_/     \\____/  /_/    /_/ \\__,_/ \\__, / \\___//_/     /_____/ \\__,_/ \\__/ \\__,_//_.___/ \\__,_//____/ \\___/\n");
    printf("                                             /____/                                                                   \n");
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("                                     Rugby Performance Metric Ltd. Player Database                                    \n");
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("                                               You are logged in as: %s                                           \n", roleName);
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("\x1b[0m"); // Reset color to default

}

// Function to Clear Terminal Screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to reset menu
void delayAndClear(int time, int role)
{
    Sleep(time);
    clearScreen();
    printHeader(role);
}

// Function to set user role
int authenticateUser(const char* username, const char* password) {
    FILE* file = fopen("login.txt", "r");
    if (file == NULL) {
        printf("Error opening login file.\n");
        return 0; // Return 0 to indicate guest role
    }

    char fileUsername[7];
    char filePassword[7];
    int role;
    int authenticated = 0; // Flag to indicate if user is authenticated

    // Read login credentials from file
    while (fscanf(file, "%6s %6s %d", fileUsername, filePassword, &role) == 3) {
        // Compare entered username and password with credentials from file
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            authenticated = 1;
            break;
        }
    }

    fclose(file);

    // Set role based on authentication status
    if (authenticated) {
        return role; // Return role of authenticated user
    }
    else {
        // Clear the terminal window
        clearScreen();
        printf("Login Failed. Continuing as Guest...\n");
        return 3; // Guest role
    }
}

// Function to display main menu and handle input
void showMainMenu(int role) {

    int choice = -1;

    do {
        printf("Please enter a command\n");

        // Menu options available for all roles
        printf("1) Display Player Details\n");
        printf("2) Generate Statistic\n");

        if (role == 1) {  // Additional options for Admin
            printf("3) Add New Player\n");
            printf("4) Display All Players\n");
            printf("5) Update Player's Statistics\n");
            printf("6) Delete Player\n");
            printf("7) Print All Players Details Into Report File\n");
        }
        else if (role == 2) { // Additional options for User
            printf("3) Update Player's Statistics\n");
            printf("4) Display All Players\n");
            printf("5) Print All Players Details Into Report File\n");
        }

        printf("0) Exit Program\n");

        printf("=> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            displayPlayerDetails(role);
            break;
        case 2:
            generateStats(role);
            break;
        case 3:
            if (role == 1) {
                addPlayer(role);
            }
            else if (role == 2) {
                editPlayer(role);
            }
            break;
        case 4:
            if (role == 1 || role == 2) {
                displayAll(role);
            }
            break;
        case 5:
            if (role == 1) {
                editPlayer(role);
            }
            else if (role == 2) {
                printReport(role);
            }
            break;
        case 6:
            if (role == 1) {
                deletePlayer(role);
            }
            break;
        case 7:
            if (role == 1) {
                printReport(role);
            }
            break;
        case 0:
            exitProgram();
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

// Function to search/display via irfu number
void displayPlayerDetails(int role) {

    delayAndClear(100, role);

    int irfuNumber;
    printf("Enter player's IRFU number to search: ");
    scanf("%d", &irfuNumber);

    PlayerDatabase* current = head;
    int found = 0;

    while (current != NULL) {
        if (current->IRFU_number == irfuNumber) {
            printf("\nPlayer Found:\n");
            printf("IRFU Number: %d\n", current->IRFU_number);
            printf("Name: %s %s\n", current->first_name, current->last_name);
            printf("Age: %d\n", current->age);
            printf("Height: %d cm\n", current->height);
            printf("Weight: %d kg\n", current->weight);
            printf("Club: %s\n", current->club);
            printf("Email: %s\n", current->email);
            printf("Position: %s\n", current->position);
            printf("Tackles Missed: %d\n", current->tackles_missed);
            printf("Meters Made: %d\n", current->meters_made);
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("No player found with IRFU number %d.\n", irfuNumber);
    }

    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function for displaying all players
void displayAll(int role)
{
    PlayerDatabase* current = head;

    if (current == NULL) {
        printf("The database is empty.\n");
    }
    else {
        printf("Displaying all players:\n\n");
        while (current != NULL) {
            printf("IRFU Number: %d\n", current->IRFU_number);
            printf("Name: %s %s\n", current->first_name, current->last_name);
            printf("Age: %d\n", current->age);
            printf("Height: %d cm\n", current->height);
            printf("Weight: %d kg\n", current->weight);
            printf("Club: %s\n", current->club);
            printf("Email: %s\n", current->email);
            printf("Position: %s\n", current->position);
            printf("Tackles Missed: %d\n", current->tackles_missed);
            printf("Meters Made: %d\n\n", current->meters_made);

            current = current->next;
        }
    }
    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function for editing players
void editPlayer(int role)
{
    int irfuNumber;
    printf("Enter player's IRFU number to edit: ");
    scanf("%d", &irfuNumber);

    PlayerDatabase* current = head;
    int found = 0;

    while (current != NULL) {
        if (current->IRFU_number == irfuNumber) {
            found = 1;
            printf("\nEditing Player:\n");

            printf("Current Age: %d. Enter new age: ", current->age);
            scanf("%d", &current->age);

            printf("Current Height: %d. Enter new height (in cm): ", current->height);
            scanf("%d", &current->height);

            printf("Current Weight: %d. Enter new weight (in kg): ", current->weight);
            scanf("%d", &current->weight);

            printf("Current Club: %s. Enter new club: ", current->club);
            scanf("%49s", current->club);

            printf("Current Position: %s. Enter new position: ", current->position);
            scanf("%19s", current->position);

            printf("Current Tackles Missed: %d. Enter new tackles missed: ", current->tackles_missed);
            scanf("%d", &current->tackles_missed);

            printf("Current Meters Made: %d. Enter new meters made: ", current->meters_made);
            scanf("%d", &current->meters_made);

            printf("\nPlayer updated successfully.\n");
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("No player found with IRFU number %d.\n", irfuNumber);
    }

    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function to add player to linked list
void addPlayer(int role) {

    delayAndClear(100, role);

    int IRFU_number, age, height, weight, tackles_missed, meters_made;
    char first_name[50], last_name[50], club[50], email[50], position[20];

    printf("Enter player's IRFU number: ");
    scanf("%d", &IRFU_number);
    printf("Enter player's first name: ");
    scanf("%49s", first_name);
    printf("Enter player's last name: ");
    scanf("%49s", last_name);
    printf("Enter player's age: ");
    scanf("%d", &age);
    printf("Enter player's height (in cm): ");
    scanf("%d", &height);
    printf("Enter player's weight (in kg): ");
    scanf("%d", &weight);
    printf("Enter player's club: ");
    scanf("%49s", club);
    printf("Enter player's email: ");
    scanf("%49s", email);
    printf("Enter player's position: ");
    scanf("%49s", position);
    printf("Enter number of tackles missed: ");
    scanf("%d", &tackles_missed);
    printf("Enter meters made: ");
    scanf("%d", &meters_made);

    // Create a new player node
    PlayerDatabase* newNode = createPlayerNode(IRFU_number, first_name, last_name, age, height, weight, club, email, position, tackles_missed, meters_made);

    // Check if new node is created successfully
    if (newNode != NULL) {
        if (head == NULL) {
            // If the list is empty, set both head and tail to the new node
            head = newNode;
            tail = newNode;
        }
        else {
            // Otherwise, append the new node to the end of the list and update tail
            tail->next = newNode;
            tail = newNode;
        }

        printf("Player added successfully!\n");
        delayAndClear(2000, role);

    }
    else {
        printf("Failed to add new player.\n");
    }

    

}

// Function to delete player based off IRFU number
void deletePlayer(int role) {
    int irfuNumber;
    printf("Enter player's IRFU number to delete: ");
    scanf("%d", &irfuNumber);

    PlayerDatabase* current = head;
    PlayerDatabase* previous = NULL;
    int found = 0;

    while (current != NULL) {
        if (current->IRFU_number == irfuNumber) {
            found = 1;

            // Deleting the head of the list
            if (previous == NULL) {
                head = current->next;
                if (head == NULL) { // If the list becomes empty
                    tail = NULL;
                }
            }
            else {
                previous->next = current->next;
                if (previous->next == NULL) { // Deleting the tail of the list
                    tail = previous;
                }
            }

            free(current); // Free the memory allocated for the node
            printf("Player with IRFU number %d deleted successfully.\n", irfuNumber);
            break;
        }

        previous = current;
        current = current->next;
    }

    if (!found) {
        printf("No player found with IRFU number %d.\n", irfuNumber);
    }

    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function to show stats
void generateStats(int role) {
    int totalPlayers = 0;
    int noTacklesMissed = 0, lessThanThreeTackles = 0, lessThanFiveTackles = 0, moreThanFiveTackles = 0;
    int noMetresMade = 0, lessThanTenMetres = 0, lessThanTwentyMetres = 0, moreThanTwentyMetres = 0;

    PlayerDatabase* current = head;

    while (current != NULL) {
        totalPlayers++;

        if (current->tackles_missed == 0) noTacklesMissed++;
        if (current->tackles_missed < 3) lessThanThreeTackles++;
        if (current->tackles_missed < 5) lessThanFiveTackles++;
        if (current->tackles_missed > 5) moreThanFiveTackles++;

        if (current->meters_made == 0) noMetresMade++;
        if (current->meters_made < 10) lessThanTenMetres++;
        if (current->meters_made < 20) lessThanTwentyMetres++;
        if (current->meters_made > 20) moreThanTwentyMetres++;

        current = current->next;
    }

    if (totalPlayers > 0) {
        printf("\nStatistics:\n");
        printf("Players who miss no tackles: %.2f%%\n", (noTacklesMissed / (double)totalPlayers) * 100);
        printf("Players who miss less than 3 tackles per match: %.2f%%\n", (lessThanThreeTackles / (double)totalPlayers) * 100);
        printf("Players who miss less than 5 tackles per match: %.2f%%\n", (lessThanFiveTackles / (double)totalPlayers) * 100);
        printf("Players who miss more than 5 tackles per match: %.2f%%\n", (moreThanFiveTackles / (double)totalPlayers) * 100);
        printf("Players who make no metres in a game: %.2f%%\n", (noMetresMade / (double)totalPlayers) * 100);
        printf("Players who make less than 10 metres in a game: %.2f%%\n", (lessThanTenMetres / (double)totalPlayers) * 100);
        printf("Players who make less than 20 metres in a game: %.2f%%\n", (lessThanTwentyMetres / (double)totalPlayers) * 100);
        printf("Players who make more than 20 metres in a game: %.2f%%\n", (moreThanTwentyMetres / (double)totalPlayers) * 100);
    }
    else {
        printf("\nNo players in database to generate statistics.\n");
    }

    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function to print report to file
void printReport(int role) {
    FILE* file = fopen("PlayerReport.txt", "w");
    if (file == NULL) {
        printf("Error opening report file.\n");
        return;
    }

    fprintf(file, "Player Details Report\n\n");

    int totalPlayers = 0;
    int noTacklesMissed = 0, lessThanThreeTackles = 0, lessThanFiveTackles = 0, moreThanFiveTackles = 0;
    int noMetresMade = 0, lessThanTenMetres = 0, lessThanTwentyMetres = 0, moreThanTwentyMetres = 0;

    PlayerDatabase* current = head;
    while (current != NULL) {
        totalPlayers++;

        // Player details
        fprintf(file, "IRFU Number: %d\n", current->IRFU_number);
        fprintf(file, "Name: %s %s\n", current->first_name, current->last_name);
        fprintf(file, "Age: %d\n", current->age);
        fprintf(file, "Height: %d cm\n", current->height);
        fprintf(file, "Weight: %d kg\n", current->weight);
        fprintf(file, "Club: %s\n", current->club);
        fprintf(file, "Email: %s\n", current->email);
        fprintf(file, "Position: %s\n", current->position);
        fprintf(file, "Tackles Missed: %d\n", current->tackles_missed);
        fprintf(file, "Meters Made: %d\n\n", current->meters_made);

        // Tally for statistics
        if (current->tackles_missed == 0) noTacklesMissed++;
        if (current->tackles_missed < 3) lessThanThreeTackles++;
        if (current->tackles_missed < 5) lessThanFiveTackles++;
        if (current->tackles_missed > 5) moreThanFiveTackles++;
        if (current->meters_made == 0) noMetresMade++;
        if (current->meters_made < 10) lessThanTenMetres++;
        if (current->meters_made < 20) lessThanTwentyMetres++;
        if (current->meters_made > 20) moreThanTwentyMetres++;

        current = current->next;
    }

    // Add player performance statistics
    fprintf(file, "\nPlayer Performance Statistics:\n");
    if (totalPlayers > 0) {
        fprintf(file, "Players who miss no tackles: %.2f%%\n", (noTacklesMissed / (double)totalPlayers) * 100);
        fprintf(file, "Players who miss less than 3 tackles per match: %.2f%%\n", (lessThanThreeTackles / (double)totalPlayers) * 100);
        fprintf(file, "Players who miss less than 5 tackles per match: %.2f%%\n", (lessThanFiveTackles / (double)totalPlayers) * 100);
        fprintf(file, "Players who miss more than 5 tackles per match: %.2f%%\n", (moreThanFiveTackles / (double)totalPlayers) * 100);
        fprintf(file, "Players who make no metres in a game: %.2f%%\n", (noMetresMade / (double)totalPlayers) * 100);
        fprintf(file, "Players who make less than 10 metres in a game: %.2f%%\n", (lessThanTenMetres / (double)totalPlayers) * 100);
        fprintf(file, "Players who make less than 20 metres in a game: %.2f%%\n", (lessThanTwentyMetres / (double)totalPlayers) * 100);
        fprintf(file, "Players who make more than 20 metres in a game: %.2f%%\n", (moreThanTwentyMetres / (double)totalPlayers) * 100);
    }
    else {
        fprintf(file, "No players in the database to generate statistics.\n");
    }

    fclose(file);
    printf("Player report has been successfully written to PlayerReport.txt\n");

    printf("Press enter to continue... ");
    getchar();
    getchar();
    delayAndClear(0, role);
}

// Function for exiting program and saving DB to file
void exitProgram() {
    // Open the file in write mode to clear its contents
    FILE* file = fopen("Rugby.txt", "w");
    if (file == NULL) {
        printf("Error opening Rugby.txt.\n");
        return;
    }

    // Iterate through the linked list and write each player's details to the file
    PlayerDatabase* current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %s %d %d %d %s %s %s %d %d\n",
            current->IRFU_number,
            current->first_name,
            current->last_name,
            current->age,
            current->height,
            current->weight,
            current->club,
            current->email,
            current->position,
            current->tackles_missed,
            current->meters_made);

        current = current->next;
    }

    fclose(file);
    printf("All player data saved to Rugby.txt. Exiting program.\n");

    // Exit the program
    exit(0);
}

void main()
{
    char username[7]; // 6 characters + 1 for null terminator
    char password[7]; // 6 characters + 1 for null terminator
    char ch;
    int i = 0;
    FILE* file;
    file = fopen("Rugby.txt", "r");
    if (file == NULL) {
        printf("Error opening Rugby.txt. Please make sure the file exists and try again.\n");
        return; // or handle the error appropriately
    }

    // Prompt the user for username
    printf("Enter username: ");
    scanf("%6s", username); // Limit input to 6 characters

    // Prompt the user for password
    printf("Enter password: ");
    while (1) {
        ch = getch(); // Read a character without echoing it to the console
        if (ch == '\r' || ch == '\n') { // If Enter key is pressed
            password[i] = '\0'; // Null-terminate the password string
            break;
        }
        else if (i < 6) { // Limit input to 6 characters
            password[i++] = ch; // Store the character in the password array
            printf("*"); // Print '*' instead of the actual character
        }
    }

    // Call the authentication function and get the role, clear screen and print the header
    int role = authenticateUser(username, password);
    delayAndClear(0, role);

    //Handle database if user or guest -> cant create new db as user or guest so if no db exists the program will end
    if (role != 1)
    {
        if (file == NULL)
        {
            printf("\nError Opening File. Login as admin to create a new database!\nExiting Program....\n");
            return;
        }
        else
        {
            readInDatabase(file);   //Users & guests cant create a new database/linked list so must be read in from file
        }
    }

    //handle database if admin -> read in database from memory or create new or exit program
    if (role == 1)
    {
        int userChoice = 0;
        do
        {
            printf("Please enter a command\n1) Create new database\n2) Read in database\n0) Exit program\n=> ");
            scanf("%d", &userChoice);

            switch (userChoice)
            {
            case 1:
                createNewDatabase(file);
                break;
            case 2:
                printf("Reading in database to memory...");
                readInDatabase(file);
                break;
            case 0:
                return;
                break;
            default:
                break;
            }

            userChoice = 0; //force userchoice to 0 to exit first menu

        } while (userChoice != 0);

        delayAndClear(2000, role);
    }

    showMainMenu(role);

}
