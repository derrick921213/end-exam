#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
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
typedef struct BPlusTreeNode
{
    unsigned long *keys;
    struct BPlusTreeNode **children;
    struct BPlusTreeNode *next;
    int n;
    bool is_leaf;
} BPlusTreeNode;
typedef struct Data {
    char id[256];
} Data;
int partition(Data *, int, int);
void quicksort(Data *, int, int);
void sort_and_write_data(Data *, int, const char *);
BPlusTreeNode *create_node(bool);
void insert(BPlusTreeNode **, const char *);
void split_child(BPlusTreeNode *, int, BPlusTreeNode *);
bool search(BPlusTreeNode*, const char *);
void free_node(BPlusTreeNode *);
void print_tree(BPlusTreeNode *, int);
void ManyFile(const int);
FILE* open_file(const char*, const char*);
void close_file(FILE*);
ParsedLine* parse_line(const char*);
void ManyFile(const int);
int isDirectoryExists(const char*);
int isFileExistsStats(const char *path);
void ProcessFile(const char *, BPlusTreeNode *, BPlusTreeNode *);
void write_data_to_screen(BPlusTreeNode *,BPlusTreeNode *);
void write_data_to_files(DataNode*,BPlusTreeNode *, BPlusTreeNode *); 
void free_data_list(DataNode*); 
void store_data(DataNode**, unsigned long, const char*,unsigned long ,const char *);
void write_data_to_screen(BPlusTreeNode *,BPlusTreeNode *);
unsigned long hash_function(const char*);
int file_separate(char *, int *, FILE **, unsigned long, const char *);
#endif