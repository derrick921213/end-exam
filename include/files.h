#ifndef FILES_H
#define FILES_H
#include "BPlusTree.h"
#include <stdio.h>
typedef struct {
    char id[32];
    char number[32];
} ParsedLine;
typedef struct DataNode {
    unsigned long student_hash_value;
    unsigned long course_hash_value;
    ParsedLine* data;
    struct DataNode *next;
} DataNode;
FILE* open_file(const char*, const char*);
void close_file(FILE*);
ParsedLine* parse_line(const char*);
void ProcessFile(const char *, BPlusTreeNode *, BPlusTreeNode*);
void ManyFile(const int);
int isDirectoryExists(const char*);
int isFileExistsStats(const char *path);

#endif 