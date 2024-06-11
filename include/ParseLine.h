#ifndef PARSELINE_H
#define PARSELINE_H
#include "Utils.h"
#include "BPlusTree.h"
#include "files.h"
#include <regex.h>
void ProcessFile(const char *, BPlusTreeNode **,BPlusTreeNode **);
ParsedLine* parse_line(const char *);
void free_parse_line(ParsedLine *line);
#endif