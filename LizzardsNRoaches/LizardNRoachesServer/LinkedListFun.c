#include "LinkedListFun.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create a new node
Node* createNode(long timestamp, int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->timestamp = timestamp;
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

// Function to add a node to the end of the list
void addToEnd(Node* head, long timestamp, int id) {
    Node* newNode = createNode(timestamp, id);
    if (head == NULL) {
        // If the list is empty, make the new node the head
        head = newNode;
    } else {
        // Traverse the list to find the last node
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Add the new node to the end
        current->next = newNode;
    }
}

// Function to remove and free the node at the top of the list
int pop(Node* head) {
    int id=0;
    if (head == NULL) {
        printf("List is empty. Cannot remove from top.\n");
        return;
    }
    Node* temp = head;
    head = head->next;
    id = temp->id;
    free(temp);
    return id;
}

// Function to check the timestamp of the first element
long checkTimestamp(Node* head) {
    if (head== NULL) {
        printf("List is empty. No timestamp to check.\n");
        exit(EXIT_FAILURE);
    }
    return head->timestamp;
}

// Function to free the memory allocated for the list
void freeList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL; // Ensure the list is empty after freeing all nodes
}