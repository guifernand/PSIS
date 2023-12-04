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
void addToEnd(long timestamp, int id,List* list) {
    Node* newNode = createNode(timestamp, id);
    Node* head = list->head;
    if (head == NULL) {
        // If the list is empty, make the new node the head
        head = newNode;
        list->len=1;
    } else {
        // Traverse the list to find the last node
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        list->len++;
        // Add the new node to the end
        current->next = newNode;
    }
}

// Function to remove and free the node at the top of the list
int pop(List* list) {
    Node* head = list->head;
    int id=0;
    if (head == NULL) {
        printf("List is empty. Cannot remove from top.\n");
        return;
    }
    Node* temp = head;
    head = head->next;
    id = temp->id;
    free(temp);
    list->len--;
    return id;
}

// Function to check the timestamp of the first element
long checkTimestamp(List* list) {
    Node* head = list->head;
    if (head== NULL) {
        printf("List is empty. No timestamp to check.\n");
        exit(EXIT_FAILURE);
    }
    return head->timestamp;
}


// Function to free the memory allocated for the list
void freeList(List* list) {
    Node* head = list->head;
    Node* current = head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL; // Ensure the list is empty after freeing all nodes
    list->len=0;
}