#ifndef LinkedList_H
#define LinkedList_H
#include "Utils.h"
#include "hashes.h"

Index *create_list_node(int);
void list_insert(Index **, int);
Index *list_search(Index *, int);
void list_delete(Index **, int);
void print_list(Index *);
void DataNode_insert(DataNode **, const char *, char *);
void DataNode_free(DataNode *);
void DataNode_write_files(DataNode *,char *);
#endif