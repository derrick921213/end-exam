// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// #define MAX_KEYS 3 // B+树的最大键数（2阶树）

// typedef struct BPlusTreeNode {
//     char **keys;
//     struct BPlusTreeNode **children;
//     struct BPlusTreeNode *next;
//     int n; // 当前键的数量
//     bool is_leaf;
// } BPlusTreeNode;
// BPlusTreeNode* create_node(bool is_leaf) {
//     BPlusTreeNode* node = (BPlusTreeNode*)malloc(sizeof(BPlusTreeNode));
//     if (!node) {
//         perror("Failed to allocate memory for BPlusTreeNode");
//         exit(EXIT_FAILURE);
//     }

//     node->keys = (char**)malloc((MAX_KEYS + 1) * sizeof(char*));
//     if (!node->keys) {
//         perror("Failed to allocate memory for keys");
//         exit(EXIT_FAILURE);
//     }

//     node->children = (BPlusTreeNode**)malloc((MAX_KEYS + 2) * sizeof(BPlusTreeNode*));
//     if (!node->children) {
//         perror("Failed to allocate memory for children");
//         exit(EXIT_FAILURE);
//     }

//     node->n = 0;
//     node->is_leaf = is_leaf;
//     node->next = NULL;
//     return node;
// }
// void insert(BPlusTreeNode** root, const char* key);
// void split_child(BPlusTreeNode* parent, int index, BPlusTreeNode* full_child);

// void insert_non_full(BPlusTreeNode* node, const char* key) {
//     int i = node->n - 1;

//     if (node->is_leaf) {
//         while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
//             node->keys[i + 1] = node->keys[i];
//             i--;
//         }
//         node->keys[i + 1] = strdup(key);
//         if (!node->keys[i + 1]) {
//             perror("Failed to allocate memory for key");
//             exit(EXIT_FAILURE);
//         }
//         node->n++;
//     } else {
//         while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
//             i--;
//         }
//         i++;
//         if (node->children[i]->n == MAX_KEYS) {
//             split_child(node, i, node->children[i]);
//             if (strcmp(key, node->keys[i]) > 0) {
//                 i++;
//             }
//         }
//         insert_non_full(node->children[i], key);
//     }
// }

// void split_child(BPlusTreeNode* parent, int index, BPlusTreeNode* full_child) {
//     int mid_index = MAX_KEYS / 2;
//     BPlusTreeNode* new_node = create_node(full_child->is_leaf);
//     new_node->n = MAX_KEYS - mid_index;

//     for (int j = 0; j < MAX_KEYS - mid_index; j++) {
//         new_node->keys[j] = full_child->keys[mid_index + j];
//     }
//     if (!full_child->is_leaf) {
//         for (int j = 0; j < MAX_KEYS - mid_index + 1; j++) {
//             new_node->children[j] = full_child->children[mid_index + j];
//         }
//     } else {
//         new_node->next = full_child->next;
//         full_child->next = new_node;
//     }

//     full_child->n = mid_index;

//     for (int j = parent->n; j >= index + 1; j--) {
//         parent->children[j + 1] = parent->children[j];
//     }
//     parent->children[index + 1] = new_node;

//     for (int j = parent->n - 1; j >= index; j--) {
//         parent->keys[j + 1] = parent->keys[j];
//     }
//     parent->keys[index] = full_child->keys[mid_index];
//     parent->n++;
// }

// void insert(BPlusTreeNode** root, const char* key) {
//     if ((*root)->n == MAX_KEYS) {
//         BPlusTreeNode* new_root = create_node(false);
//         new_root->children[0] = *root;
//         split_child(new_root, 0, *root);
//         insert_non_full(new_root, key);
//         *root = new_root;
//     } else {
//         insert_non_full(*root, key);
//     }
// }
// bool search(BPlusTreeNode* node, const char* key) {
//     int i = 0;
//     while (i < node->n && strcmp(key, node->keys[i]) > 0) {
//         i++;
//     }
//     if (i < node->n && strcmp(key, node->keys[i]) == 0 && node->is_leaf) {
//         return true;
//     }
//     if (node->is_leaf) {
//         return false;
//     }
//     return search(node->children[i], key);
// }
// void range_query(BPlusTreeNode* node, const char* start_key, const char* end_key) {
//     while (node != NULL && strcmp(node->keys[0], start_key) < 0) {
//         node = node->next;
//     }

//     while (node != NULL && strcmp(node->keys[0], end_key) <= 0) {
//         for (int i = 0; i < node->n && strcmp(node->keys[i], end_key) <= 0; i++) {
//             if (strcmp(node->keys[i], start_key) >= 0) {
//                 printf("%s ", node->keys[i]);
//             }
//         }
//         node = node->next;
//     }
//     printf("\n");
// }
// void free_node(BPlusTreeNode* node) {
//     if (!node) return;
//     if (!node->is_leaf) {
//         for (int i = 0; i <= node->n; i++) {
//             free_node(node->children[i]);
//         }
//     }
//     for (int i = 0; i < node->n; i++) {
//         free(node->keys[i]);
//     }
//     free(node->keys);
//     free(node->children);
//     free(node);
// }
// int main() {
//     BPlusTreeNode* root = create_node(true);

//     insert(&root, "妳好");
//     insert(&root, "world");
//     insert(&root, "B+Tree");
//     insert(&root, "example");
//     insert(&root, "insert");
//     insert(&root, "search");

//     printf("Searching for 'world': %s\n", search(root, "world") ? "Found" : "Not Found");
//     printf("Searching for 'B+Tree': %s\n", search(root, "B+Tree") ? "Found" : "Not Found");
//     printf("Searching for 'missing': %s\n", search(root, "missing") ? "Found" : "Not Found");

//     printf("Range query from 'B' to 'i': ");
//     range_query(root, "B", "i");

//     free_node(root);

//     return 0;
// }

