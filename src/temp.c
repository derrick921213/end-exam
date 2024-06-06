#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_KEYS 3 // B+ 樹的最大鍵數（2階樹）

typedef struct BPlusTreeNode {
    unsigned long *keys;  // 存儲哈希值
    struct BPlusTreeNode **children;
    struct BPlusTreeNode *next;
    int n; // 當前鍵的數量
    bool is_leaf;
} BPlusTreeNode;

typedef struct DataNode {
    unsigned long hash_value;
    char course_id[100];
    struct DataNode *next;
} DataNode;

unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

BPlusTreeNode* create_node(bool is_leaf) {
    BPlusTreeNode* node = (BPlusTreeNode*)malloc(sizeof(BPlusTreeNode));
    if (!node) {
        perror("Failed to allocate memory for BPlusTreeNode");
        exit(EXIT_FAILURE);
    }

    node->keys = (unsigned long*)malloc((MAX_KEYS + 1) * sizeof(unsigned long));
    if (!node->keys) {
        perror("Failed to allocate memory for keys");
        exit(EXIT_FAILURE);
    }

    node->children = (BPlusTreeNode**)malloc((MAX_KEYS + 2) * sizeof(BPlusTreeNode*));
    if (!node->children) {
        perror("Failed to allocate memory for children");
        exit(EXIT_FAILURE);
    }

    node->n = 0;
    node->is_leaf = is_leaf;
    node->next = NULL;
    return node;
}

void insert(BPlusTreeNode** root, const char* key);
void split_child(BPlusTreeNode* parent, int index, BPlusTreeNode* full_child);

void insert_non_full(BPlusTreeNode* node, unsigned long key) {
    int i = node->n - 1;

    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->n == MAX_KEYS) {
            split_child(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key);
    }
}

void split_child(BPlusTreeNode* parent, int index, BPlusTreeNode* full_child) {
    int mid_index = MAX_KEYS / 2;
    BPlusTreeNode* new_node = create_node(full_child->is_leaf);
    new_node->n = MAX_KEYS - mid_index - 1;

    for (int j = 0; j < MAX_KEYS - mid_index - 1; j++) {
        new_node->keys[j] = full_child->keys[mid_index + 1 + j];
    }
    if (!full_child->is_leaf) {
        for (int j = 0; j < MAX_KEYS - mid_index; j++) {
            new_node->children[j] = full_child->children[mid_index + 1 + j];
        }
    } else {
        new_node->next = full_child->next;
        full_child->next = new_node;
    }

    full_child->n = mid_index;

    for (int j = parent->n; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = new_node;

    for (int j = parent->n - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[index] = full_child->keys[mid_index];
    parent->n++;
}

void insert(BPlusTreeNode** root, const char* key) {
    unsigned long hash_key = hash_function(key);
    if ((*root)->n == MAX_KEYS) {
        BPlusTreeNode* new_root = create_node(false);
        new_root->children[0] = *root;
        split_child(new_root, 0, *root);
        insert_non_full(new_root, hash_key);
        *root = new_root;
    } else {
        insert_non_full(*root, hash_key);
    }
}

bool search(BPlusTreeNode* node, const char* key) {
    unsigned long hash_key = hash_function(key);
    int i = 0;
    while (i < node->n && hash_key > node->keys[i]) {
        i++;
    }
    if (i < node->n && hash_key == node->keys[i]) {
        return true;
    }
    if (node->is_leaf) {
        BPlusTreeNode* next_leaf = node->next;
        while (next_leaf != NULL) {
            for (int j = 0; j < next_leaf->n; j++) {
                if (next_leaf->keys[j] == hash_key) {
                    return true;
                }
            }
            next_leaf = next_leaf->next;
        }
        return false;
    }
    return search(node->children[i], key);
}

void free_node(BPlusTreeNode* node) {
    if (!node) return;
    if (!node->is_leaf) {
        for (int i = 0; i <= node->n; i++) {
            free_node(node->children[i]);
        }
    }
    free(node->keys);
    free(node->children);
    free(node);
}

void print_tree(BPlusTreeNode* node, int level) {
    if (node == NULL) return;

    printf("Level %d: ", level);
    for (int i = 0; i < node->n; i++) {
        printf("%lu ", node->keys[i]);
    }
    printf("\n");

    if (!node->is_leaf) {
        for (int i = 0; i <= node->n; i++) {
            print_tree(node->children[i], level + 1);
        }
    }
}

// 新的函式，將資料存儲到記憶體中的連結串列
void store_data(DataNode** head, unsigned long hash_value, const char* course_id) {
    DataNode* new_node = (DataNode*)malloc(sizeof(DataNode));
    if (!new_node) {
        perror("Failed to allocate memory for DataNode");
        exit(EXIT_FAILURE);
    }
    new_node->hash_value = hash_value;
    strcpy(new_node->course_id, course_id);
    new_node->next = *head;
    *head = new_node;
}

// 將連結串列中的資料寫入到對應的檔案中
void write_data_to_files(DataNode* head) {
    DataNode* current = head;
    while (current) {
        char filename[256];
        sprintf(filename, "../index/%lu_hash", current->hash_value);
        FILE *file = fopen(filename, "a"); // 使用追加模式
        if (!file) {
            printf("無法創建檔案 %s\n", filename);
            current = current->next;
        }
        fprintf(file, "%s\n", current->course_id); // 將課程ID寫入哈希文件
        fclose(file);
        current = current->next;
    }
}

// 釋放連結串列所佔用的記憶體
void free_data_list(DataNode* head) {
    DataNode* current = head;
    while (current) {
        DataNode* next = current->next;
        free(current);
        current = next;
    }
}

int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}

int main() {
    BPlusTreeNode* root = create_node(true);
    char Filename[100];

    FILE *File;
    
    if(isDirectoryExists("../index")){
        printf("資料夾已存在\n");
    }
    else{
        printf("資料夾不存在\n");
        exit(1);
    }

    for (int i = 1; i <= 10; i++) {
        DataNode* data_list = NULL; // 初始化一個新的連結串列
        sprintf(Filename, "../data/data_no_cname/%04d", i);  // 將數字 i 格式化為四位數的字串，並存儲到 Filename 中

        File = fopen(Filename, "r");  // "r" 表示讀取模式
        if (File == NULL) {
            printf("無法打開檔案 %s\n", Filename);
            continue;
        }
        printf("正在讀取檔案 %s\n", Filename);
        char line[256];
        while (fgets(line, sizeof(line), File)) {
            char student_id[100], course_id[100];
            sscanf(line, "%[^,],%s", student_id, course_id);
            unsigned long hash_value = hash_function(student_id);
            insert(&root, student_id);

            // 將資料存儲到記憶體中的連結串列
            store_data(&data_list, hash_value, course_id);
        }

        fclose(File);

        // 將存儲在記憶體中的資料寫入到對應的檔案中
        write_data_to_files(data_list);

        // 釋放連結串列所佔用的記憶體
        free_data_list(data_list);
    }

    // 打印B+樹的結構
    print_tree(root, 0);

    // 釋放B+樹節點的記憶體
    free_node(root);

    return 0;
}
