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
#else
#include <unistd.h>
#include <limits.h>
#endif

#define INDEXDIR "data/index"
#define TREEDIR "data/trees"
#define TREEFILE TREEDIR "/bplustree"
#define DATA "data"
#define STUDENT_COURSDE INDEXDIR"/student_To_course"
#define COURSE_STUDENT INDEXDIR"/course_To_student"
typedef struct Node
{
    int data;
    struct Node *next;
} Node;
typedef struct DataNode
{
    unsigned long hash_value;
    struct ParsedLine *data;
    struct DataNode *next;
} DataNode;
typedef struct ParsedLine
{
    char id[32];
    int number;
} ParsedLine;
typedef struct Data {
    char id[256];
} Data;
#endif