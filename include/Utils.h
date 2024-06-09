#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#define CLEAR "cls"
#else
#include <unistd.h>
#include <limits.h>
#define CLEAR "clear"
#endif

#define INDEXDIR "data/index"
#define TREEDIR "data/trees"
#define TREEFILE TREEDIR "/bplustree"
#define DATA "data"
#define STUDENT_COURSDE INDEXDIR"/student_To_course"
#define COURSE_STUDENT INDEXDIR"/course_To_student"
#define STUDENT_TO_STUDENT_HASH INDEXDIR"/student_To_student_hash"
#define COURSE_TO_COURSE_HASH INDEXDIR"/course_To_course_hash"
#define PLACEHOLDER "Selection Action:\n1. Search Student\n2. Search Course\n0. Quit\n>> "
typedef struct DataNode
{
    unsigned long hash_value;
    struct ParsedLine *data;
    struct DataNode *next;
} DataNode;
typedef struct ParsedLine
{
    char id[32];
    char number[50];
} ParsedLine;
typedef struct Data {
    char id[256];
} Data;
typedef struct INDEX
{
    char id[256];
    char hash_localtion[256];
    struct INDEX *next;
} INDEX;
void printColored(FILE *,const char *, const char *);
void clearScreen(void);
#endif