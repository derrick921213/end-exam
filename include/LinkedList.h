#ifndef LinkedList_H
#define LinkedList_H
#include "Utils.h"
#include "hashes.h"
#include "files.h"
#define MAX_FILE_SIZE 2048
void DataNode_insert(DataNode **, const char *, char *);
void DataNode_free(DataNode *);
void DataNode_write_files(DataNode *, char *, char *);
void DataNode_write_index(DataNode *, char *, char *,int *);
#endif