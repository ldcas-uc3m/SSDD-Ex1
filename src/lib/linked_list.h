#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_


struct Node {
    int key;
    char* value1;
    int value2;
    double value3;
    struct Node *next;
};

typedef struct Node* List;

// sync list concurrency
pthread_mutex_t mutex_l;


int init(List* l);
int set(List* l, int key, char* value1, int value2, double value3);
int get(List l, int key, char* value1, int* value2, double* value3);
int exist(List l, int key);
int modify(List* l, int key, char* value1, int value2, double value3);
int copyKey(List* l, int key1, int key2);
int printList(List l);
int deleteNode(List* l, int key);
int destroy(List* l);

#endif