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
#endif