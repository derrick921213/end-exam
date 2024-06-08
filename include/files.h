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
typedef struct Data {
    char id[256];
} Data;
FILE* open_file(const char*, const char*);
void close_file(FILE*);
ParsedLine* parse_line(const char*);
void ProcessFile(const char *, BPlusTreeNode *, BPlusTreeNode*);
void ManyFile(const int);
int isDirectoryExists(const char*);
int isFileExistsStats(const char *path);
void write_data_to_files(DataNode*); 
void free_data_list(DataNode*); 
void store_data(DataNode**, unsigned long, const char*,unsigned long ,const char *);
void write_data_to_screen(BPlusTreeNode *,BPlusTreeNode *);
int partition(Data *, int, int);
void quicksort(Data *, int, int);
void sort_and_write_data(Data *, int, const char *);
#endif 