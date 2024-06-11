#ifndef PARSELINE_H
#define PARSELINE_H
#include "Utils.h"
#include "BPlusTree.h"
#include "files.h"
#include <regex.h>
void ProcessFile(const char *, BPlusTreeNode **,BPlusTreeNode **);
int parse_line(const char *, ParsedLine *);
void free_parse_line(ParsedLine *line);
#endif