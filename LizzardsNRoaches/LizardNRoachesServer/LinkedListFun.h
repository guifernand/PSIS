#ifndef LINKEDLIST
#define LINKEDLIST


typedef struct Node {
    long timestamp;
    int id;
    struct Node* next;
} Node;

Node* createNode(long timestamp, int id);

void addToEnd(Node* head, long timestamp, int id);

int pop(Node* head);

long checkTimestamp(Node* head);

void freeList(Node* head);

#endif