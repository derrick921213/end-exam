#ifndef FILES_H
#define FILES_H
#include <stdio.h>
typedef struct {
    char id[32];
    int number;
} ParsedLine;
FILE* open_file(const char*, const char*);
void close_file(FILE*);
ParsedLine* parse_line(const char*);
void ProcessFile(const char*);
void ManyFile(const int);
#endif 