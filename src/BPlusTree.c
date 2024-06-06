#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 3
typedef struct Node
{
    int keys[MAX];
    struct Node *children[MAX + 1];
    struct Node *next;
    bool leaf;
    int num_keys;
} Node;

typedef struct BPlusTree
{
    Node *root;
} BPlusTree;
Node* create_node(bool is_leaf){
    Node *node = (Node *)malloc(sizeof(Node));
    node->num_keys = 0;
    node->leaf = is_leaf;
    node->next = NULL;
    for (int i = 0; i < MAX+1; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

BPlusTree* createBPlusTree(){
    BPlusTree *tree = (BPlusTree *)malloc(sizeof(BPlusTree));
    tree->root = create_node(true);
    return tree;
}

void split_node(Node* parent,int index,Node* node){
    Node* new_node = create_node(node->leaf);
    new_node->num_keys = (MAX-1)/2;
    // 將原節點的後半複製到新節點
    for(int i=0;i<(MAX-1)/2;i++){
        new_node->keys[i] = node->keys[i+(MAX-1)/2];
    }
    if(!node->leaf){
        for(int i=0;i<(MAX+1)/2;i++){
            new_node->children[i] = node->children[i+(MAX+1)/2];
        }
    }
    else{
        new_node->next = node->next;
        node->next = new_node;
    }
    node->num_keys = (MAX-1)/2;
    for (int i = parent->num_keys; i > index; i--) {
        parent->children[i + 1] = parent->children[i];
        parent->keys[i] = parent->keys[i - 1];
    }
    parent->children[index + 1] = new_node;
    parent->keys[index] = node->keys[(MAX - 1) / 2];
    parent->num_keys++;
}
// 插入一个键到节点
void insert_non_full(Node *node, int key) {
    int i = node->num_keys - 1;
    if (node->leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == MAX) {
            split_node(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

// 插入一个键到B+树
void insert(BPlusTree *tree, int key) {
    Node *root = tree->root;
    if (root->num_keys == MAX) {
        Node *new_root = create_node(0);
        new_root->children[0] = root;
        split_node(new_root, 0, root);
        tree->root = new_root;
        insert_non_full(new_root, key);
    } else {
        insert_non_full(root, key);
    }
}

// 遍历B+树
void traverse(Node *node) {
    if (node != NULL) {
        int i;
        for (i = 0; i < node->num_keys; i++) {
            if (!node->leaf) {
                traverse(node->children[i]);
            }
            printf("%d ", node->keys[i]);
        }
        if (!node->leaf) {
            traverse(node->children[i]);
        }
    }
}

// 主函数示例
int main() {
    BPlusTree *tree = createBPlusTree();
    insert(tree, 10);
    insert(tree, 20);
    insert(tree, 5);
    insert(tree, 6);
    insert(tree, 12);
    insert(tree, 30);
    insert(tree, 7);
    insert(tree, 17);

    printf("B+ Tree traversal:\n");
    traverse(tree->root);
    printf("\n");

    return 0;
}