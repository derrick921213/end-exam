#ifndef LinkedList_H
#define LinkedList_H
#include "Utils.h"
#include "hashes.h"

Node *create_list_node(int);
void list_insert(Node **, int);
Node *list_search(Node *, int);
void list_delete(Node **, int);
void print_list(Node *);
void DataNode_insert(DataNode **, const char *, char *);
void DataNode_free(DataNode *);
void DataNode_write_files(DataNode *,char *);
#endif