#ifndef FILES_H
#define FILES_H
#include <sys/types.h>
#include <sys/stat.h>
#include "BPlusTree.h"
#include "Utils.h"
#include "hashes.h"
typedef struct
{
    char id[32];
    int number;
} ParsedLine;
typedef struct DataNode
{
    unsigned long hash_value;
    ParsedLine *data;
    struct DataNode *next;
} DataNode;
FILE *open_file(const char *, const char *);
void close_file(FILE *);
ParsedLine *parse_line(const char *);
void ProcessFile(const char *, BPlusTreeNode **);
void ManyFile(const int);
int isDirectoryExists(const char *);
int isFileExistsStats(const char *path);
void write_data_to_files(DataNode *);
void free_data_list(DataNode *);
void printProgressBar(int, int);
void store_data(DataNode **, unsigned long, const char *, int);
int create_directory(const char *);
int delete_directory(const char *);
#endif