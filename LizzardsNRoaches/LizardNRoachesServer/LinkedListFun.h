#ifndef LINKEDLIST
#define LINKEDLIST


typedef struct Node {
    long timestamp;
    int id;
    struct Node* next;
} Node;

typedef struct List {
    int len;
    struct Node* head;
} List;

Node* createNode(long timestamp, int id);

void addToEnd(List* head, long timestamp, int id);

int pop(List* head);

long checkTimestamp(List* head);

void freeList(List* head);

#endif