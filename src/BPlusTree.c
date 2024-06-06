#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "BPlusTree.h"
#include "hashes.h"
#define MAX_KEYS 3

BPlusTreeNode *create_node(bool is_leaf)
{
    BPlusTreeNode *node = (BPlusTreeNode *)malloc(sizeof(BPlusTreeNode));
    if (!node)
    {
        perror("Failed to allocate memory for BPlusTreeNode");
        exit(EXIT_FAILURE);
    }

    node->keys = (unsigned long *)malloc((MAX_KEYS + 1) * sizeof(unsigned long));
    if (!node->keys)
    {
        perror("Failed to allocate memory for keys");
        exit(EXIT_FAILURE);
    }

    node->children = (BPlusTreeNode **)malloc((MAX_KEYS + 2) * sizeof(BPlusTreeNode *));
    if (!node->children)
    {
        perror("Failed to allocate memory for children");
        exit(EXIT_FAILURE);
    }
    node->n = 0;
    node->is_leaf = is_leaf;
    node->next = NULL;
    return node;
}

void insert_non_full(BPlusTreeNode *node, unsigned long key)
{
    int i = node->n - 1;

    if (node->is_leaf)
    {
        while (i >= 0 && key < node->keys[i])
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    }
    else
    {
        while (i >= 0 && key < node->keys[i])
        {
            i--;
        }
        i++;
        if (node->children[i]->n == MAX_KEYS)
        {
            split_child(node, i, node->children[i]);
            if (key > node->keys[i])
            {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

void split_child(BPlusTreeNode *parent, int index, BPlusTreeNode *full_child)
{
    int mid_index = MAX_KEYS / 2;
    BPlusTreeNode *new_node = create_node(full_child->is_leaf);
    new_node->n = MAX_KEYS - mid_index - 1;

    for (int j = 0; j < MAX_KEYS - mid_index - 1; j++)
    {
        new_node->keys[j] = full_child->keys[mid_index + 1 + j];
    }
    if (!full_child->is_leaf)
    {
        for (int j = 0; j < MAX_KEYS - mid_index; j++)
        {
            new_node->children[j] = full_child->children[mid_index + 1 + j];
        }
    }
    else
    {
        new_node->next = full_child->next;
        full_child->next = new_node;
    }

    full_child->n = mid_index;

    for (int j = parent->n; j >= index + 1; j--)
    {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = new_node;

    for (int j = parent->n - 1; j >= index; j--)
    {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[index] = full_child->keys[mid_index];
    parent->n++;
}

void insert(BPlusTreeNode **root, const char *key)
{
    unsigned long hash_key = hash_function(key);
    // printf("Inserting key: %s, Hash: %lu\n", key, hash_key);
    if ((*root)->n == MAX_KEYS)
    {
        BPlusTreeNode *new_root = create_node(false);
        new_root->children[0] = *root;
        split_child(new_root, 0, *root);
        insert_non_full(new_root, hash_key);
        *root = new_root;
    }
    else
    {
        insert_non_full(*root, hash_key);
    }
}

bool search(BPlusTreeNode *node, const char *key)
{
    unsigned long hash_key = hash_function(key);
    // printf("Searching key: %s, Hash: %lu\n", key, hash_key);
    int i = 0;
    while (i < node->n && hash_key > node->keys[i])
    {
        i++;
    }
    if (i < node->n && hash_key == node->keys[i])
    {
        return true;
    }
    if (node->is_leaf)
    {
        // 当到达叶子节点时，检查下一个叶子节点
        BPlusTreeNode *next_leaf = node->next;
        while (next_leaf != NULL)
        {
            for (int j = 0; j < next_leaf->n; j++)
            {
                if (next_leaf->keys[j] == hash_key)
                {
                    return true;
                }
            }
            next_leaf = next_leaf->next;
        }
        return false;
    }
    return search(node->children[i], key);
}

void free_node(BPlusTreeNode *node)
{
    if (!node)
        return;
    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->n; i++)
        {
            free_node(node->children[i]);
        }
    }
    free(node->keys);
    free(node->children);
    free(node);
}

void print_tree(BPlusTreeNode *node, int level)
{
    if (node == NULL)
        return;

    // 打印当前节点
    printf("Level %d: ", level);
    for (int i = 0; i < node->n; i++)
    {
        printf("%lu ", node->keys[i]);
    }
    printf("\n");

    // 递归打印子节点
    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->n; i++)
        {
            print_tree(node->children[i], level + 1);
        }
    }
}