#ifndef LinkedList_H
#define LinkedList_H
#include "Utils.h"
#include "files.h"
typedef struct Node {
    int data;
    struct Node* next;
} Node;
typedef struct DataNode
{
    unsigned long hash_value;
    struct ParsedLine *data;
    struct DataNode *next;
} DataNode;
Node *create_node(int);
void list_insert(Node **, int);
Node *list_search(Node *, int);
void list_delete(Node **, int);
void print_list(Node*);
#endif