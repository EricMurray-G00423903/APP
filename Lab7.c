#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char studentName[50];
    int studentAge;
    char studentID[20];
    float averageGrade;
    struct node* NEXT;
} nodeT;

void addStudentAtStart(nodeT** headPtr) {
    nodeT* newNode = (nodeT*)malloc(sizeof(nodeT));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter student name: ");
    scanf("%s", newNode->studentName);
    printf("Enter student age: ");
    scanf("%d", &newNode->studentAge);
    printf("Enter student ID: ");
    scanf("%s", newNode->studentID);
    printf("Enter student average grade: ");
    scanf("%f", &newNode->averageGrade);

    newNode->NEXT = *headPtr;
    *headPtr = newNode;
}

void addStudentAtEnd(nodeT** headPtr) {
    nodeT* newNode = (nodeT*)malloc(sizeof(nodeT));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter student name: ");
    scanf("%s", newNode->studentName);
    printf("Enter student age: ");
    scanf("%d", &newNode->studentAge);
    printf("Enter student ID: ");
    scanf("%s", newNode->studentID);
    printf("Enter student average grade: ");
    scanf("%f", &newNode->averageGrade);
    newNode->NEXT = NULL;

    if (*headPtr == NULL) {
        *headPtr = newNode;
    } else {
        nodeT* last = *headPtr;
        while (last->NEXT != NULL) {
            last = last->NEXT;
        }
        last->NEXT = newNode;
    }
}

void addStudentAtPosition(nodeT** headPtr, int position) {
    if (position < 1) {
        printf("Invalid position!\n");
        return;
    }

    if (position == 1) {
        addStudentAtStart(headPtr);
        return;
    }

    nodeT* newNode = (nodeT*)malloc(sizeof(nodeT));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter student name: ");
    scanf("%s", newNode->studentName);
    printf("Enter student age: ");
    scanf("%d", &newNode->studentAge);
    printf("Enter student ID: ");
    scanf("%s", newNode->studentID);
    printf("Enter student average grade: ");
    scanf("%f", &newNode->averageGrade);

    nodeT* current = *headPtr;
    for (int i = 1; i < position - 1; i++) {
        if (current == NULL) {
            printf("Position is beyond the list length!\n");
            free(newNode);
            return;
        }
        current = current->NEXT;
    }

    newNode->NEXT = current->NEXT;
    current->NEXT = newNode;
}

void deleteStudentAtStart(nodeT** headPtr) {
    if (*headPtr == NULL) {
        printf("List is already empty!\n");
        return;
    }

    nodeT* temp = *headPtr;
    *headPtr = (*headPtr)->NEXT;
    free(temp);
}

void deleteStudentAtEnd(nodeT** headPtr) {
    if (*headPtr == NULL) {
        printf("List is already empty!\n");
        return;
    }

    if ((*headPtr)->NEXT == NULL) {
        deleteStudentAtStart(headPtr);
        return;
    }

    nodeT *current = *headPtr, *prev = NULL;
    while (current->NEXT != NULL) {
        prev = current;
        current = current->NEXT;
    }

    prev->NEXT = NULL;
    free(current);
}

void deleteStudentAtPosition(nodeT** headPtr, int position) {
    if (*headPtr == NULL) {
        printf("List is empty!\n");
        return;
    }

    if (position < 1) {
        printf("Invalid position!\n");
        return;
    }

    if (position == 1) {
        deleteStudentAtStart(headPtr);
        return;
    }

    nodeT *current = *headPtr, *prev = NULL;
    for (int i = 1; i < position; i++) {
        if (current == NULL) {
            printf("Position is beyond the list length!\n");
            return;
        }
        prev = current;
        current = current->NEXT;
    }

    if (current == NULL) {
        printf("Position is beyond the list length!\n");
        return;
    }

    prev->NEXT = current->NEXT;
    free(current);
}

void displayList(nodeT* headPtr) {
    if (headPtr == NULL) {
        printf("List is empty!\n");
        return;
    }

    nodeT* temp = headPtr;
    while (temp != NULL) {
        printf("Student Name: %s\n", temp->studentName);
        printf("Student Age: %d\n", temp->studentAge);
        printf("Student ID: %s\n", temp->studentID);
        printf("Average Grade: %.2f\n\n", temp->averageGrade);
        temp = temp->NEXT;
    }
}

void main() {
    nodeT* headPtr = NULL;
    int choice, position;

    // Interactive part of the program
    do {
        printf("Please enter 1 to add a student at the beginning of the list\n");
        printf("Please enter 2 to add a student at the end of the list\n");
        printf("Please enter 3 to add a student at a specific position in the list\n");
        printf("Please enter 4 to display the list\n");
        printf("Please enter 5 to delete a student from the start of the list\n");
        printf("Please enter 6 to delete a student from the end of the list\n");
        printf("Please enter 7 to delete a student from a specific position in the list\n");
        printf("Please enter -1 to terminate\n");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addStudentAtStart(&headPtr);
                break;
            case 2:
                addStudentAtEnd(&headPtr);
                break;
            case 3:
                printf("Enter the position to add the student\n");
                scanf("%d", &position);
                addStudentAtPosition(&headPtr, position);
                break;
            case 4:
                displayList(headPtr);
                break;
            case 5:
                deleteStudentAtStart(&headPtr);
                break;
            case 6:
                deleteStudentAtEnd(&headPtr);
                break;
            case 7:
                printf("Enter the position to delete the student\n");
                scanf("%d", &position);
                deleteStudentAtPosition(&headPtr, position);
                break;
            case -1:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != -1);
}
