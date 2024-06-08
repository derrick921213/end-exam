#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <stdbool.h>

#define MAX_KEYS 3

typedef struct BPlusTreeNode {
    unsigned long *keys;
    struct BPlusTreeNode **children;
    struct BPlusTreeNode *next;
    int n;
    bool is_leaf;
} BPlusTreeNode;

BPlusTreeNode *create_node(bool is_leaf);
void insert(BPlusTreeNode **root, const char *key);
bool search(BPlusTreeNode *node, const char *key);
void free_node(BPlusTreeNode *node);
void print_tree(BPlusTreeNode *node, int level);
void save_tree(const char *filename, BPlusTreeNode *root);
BPlusTreeNode *load_tree(const char *filename);
void split_child(BPlusTreeNode *, int , BPlusTreeNode *);

#endif // BPLUSTREE_H
