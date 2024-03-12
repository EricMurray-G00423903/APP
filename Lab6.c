#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int value;
	struct node* NEXT;
} nodeT;

// Function to add an element at the top of the list
void addTop(nodeT** headPtr, int value) {
	nodeT* temp = (nodeT*)malloc(sizeof(nodeT));
	temp->value = value;
	temp->NEXT = *headPtr;
	*headPtr = temp;
}

// Function to add an element at the end of the list
void addEnd(nodeT** headPtr, int value) {
	nodeT* newNode = (nodeT*)malloc(sizeof(nodeT));
	newNode->value = value;
	newNode->NEXT = NULL;

	if (*headPtr == NULL) {
		*headPtr = newNode;
	}
	else {
		nodeT* temp = *headPtr;
		while (temp->NEXT != NULL) {
			temp = temp->NEXT;
		}
		temp->NEXT = newNode;
	}
}

// Function to delete an element from the list
void deleteNode(nodeT** headPtr, int key) {
	nodeT* temp = *headPtr, * prev = NULL;

	if (temp != NULL && temp->value == key) {
		*headPtr = temp->NEXT;
		free(temp);
		return;
	}

	while (temp != NULL && temp->value != key) {
		prev = temp;
		temp = temp->NEXT;
	}

	if (temp == NULL) return;

	prev->NEXT = temp->NEXT;
	free(temp);
}

// Function to display the contents of the list
void displayList(nodeT* headPtr) {
	nodeT* temp = headPtr;
	while (temp != NULL) {
		printf("The value at this node is %d\n", temp->value);
		temp = temp->NEXT;
	}
}

// Function to get the length of the list
int getListLength(nodeT* headPtr) {
	int length = 0;
	nodeT* temp = headPtr;
	while (temp != NULL) {
		length++;
		temp = temp->NEXT;
	}
	return length;
}

// Function to search an element in the list
int searchList(nodeT* headPtr, int key) {
	nodeT* temp = headPtr;
	while (temp != NULL) {
		if (temp->value == key) {
			return 1;
		}
		temp = temp->NEXT;
	}
	return 0;
}

// Main function
void main() {
	nodeT* headPtr = NULL;
	int choice, value;

	do {
		printf("Please enter 1 to add an element at the beginning of the list\n");
		printf("Please enter 2 to add an element at the end of the list\n");
		printf("Please enter 3 to display the list\n");
		printf("Please enter 4 to delete an element from the list\n");
		printf("Please enter 5 to display the length of the list\n");
		printf("Please enter 6 to search for a value in the list\n");
		printf("Please enter -1 to terminate\n");
		scanf("%d", &choice);

		switch (choice) {
		case 1:
			printf("Please enter the value for the element\n");
			scanf("%d", &value);
			addTop(&headPtr, value);
			break;
		case 2:
			printf("Please enter the value for the element\n");
			scanf("%d", &value);
			addEnd(&headPtr, value);
			break;
		case 3:
			displayList(headPtr);
			break;
		case 4:
			printf("Please enter the value of the element to delete\n");
			scanf("%d", &value);
			deleteNode(&headPtr, value);
			break;
		case 5:
			printf("Length of the list is: %d\n", getListLength(headPtr));
			break;
		case 6:
			printf("Please enter the value to search for\n");
			scanf("%d", &value);
			if (searchList(headPtr, value)) {
				printf("Element %d is present in the list\n", value);
			}
			else {
				printf("Element %d is not present in the list\n", value);
			}
			break;
		case -1:
			break;
		default:
			printf("Invalid choice. Please try again.\n");
		}
	} while (choice != -1);
}
