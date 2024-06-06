#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <stdbool.h>

typedef struct BPlusTreeNode
{
    unsigned long *keys;
    struct BPlusTreeNode **children;
    struct BPlusTreeNode *next;
    int n;
    bool is_leaf;
} BPlusTreeNode;
BPlusTreeNode *create_node(bool);
void insert(BPlusTreeNode **, const char *);
void split_child(BPlusTreeNode *, int, BPlusTreeNode *);
bool search(BPlusTreeNode*, const char *);
void free_node(BPlusTreeNode *);
void print_tree(BPlusTreeNode *, int);
#endif // BPLUSTREE_H