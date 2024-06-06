#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define NUM_SHARDS 10
#define T 3
typedef struct BTreeNode {
    uint32_t *keys;
    long *offsets;
    int t;
    struct BTreeNode **C;
    int n;
    int leaf;
} BTreeNode;
typedef struct BPlusTreeNode {
    uint32_t *keys;
    char **values;
    int t;
    struct BPlusTreeNode **C;
    int n;
    int leaf;
    struct BPlusTreeNode *next;
} BPlusTreeNode;
typedef struct {
    BTreeNode *indexRoot;
    BPlusTreeNode *dataRoot;
} Database;
uint32_t hashFunction(const char *str);
void writeDataToShardedFile(const char *key, const char *data);
BTreeNode *createNode(int t, int leaf);
void insertNonFull(BTreeNode *x, uint32_t k, long offset);
void splitChild(BTreeNode *x, int i, BTreeNode *y);
void insert(BTreeNode **root, uint32_t k, long offset);
void writeCompressedDataToDisk(const char *filename, const char *data, uint32_t data_len);
char *readCompressedDataFromDisk(const char *filename, uint32_t data_len);
BPlusTreeNode *createBPlusNode(int t, int leaf);
Database *createDatabase();
void insertToDatabase(Database *db, const char *key, const char *data);
char *searchInDatabase(Database *db, const char *key);
BTreeNode *search(BTreeNode *root, uint32_t k);

uint32_t hashFunction(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++))
        hash = hash * 33 + c;
    return hash;
}
void writeDataToShardedFile(const char *key, const char *data) {
    uint32_t hashValue = hashFunction(key);
    int shardId = hashValue % NUM_SHARDS;
    char filename[50];
    sprintf(filename, "shard_%d.dat", shardId);

    FILE *file = fopen(filename, "ab");
    if (file != NULL) {
        uint32_t key_len = strlen(key);
        uint32_t data_len = strlen(data);
        fwrite(&key_len, sizeof(uint32_t), 1, file);
        fwrite(key, sizeof(char), key_len, file);
        fwrite(&data_len, sizeof(uint32_t), 1, file);
        fwrite(data, sizeof(char), data_len, file);
        fclose(file);
    } else {
        perror("File opening failed");
    }
}
BTreeNode *createNode(int t, int leaf) {
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (uint32_t *)malloc((2 * t - 1) * sizeof(uint32_t));
    node->offsets = (long *)malloc((2 * t - 1) * sizeof(long));
    node->C = (BTreeNode **)malloc(2 * t * sizeof(BTreeNode *));
    node->n = 0;
    return node;
}

void insertNonFull(BTreeNode *x, uint32_t k, long offset) {
    int i = x->n - 1;
    if (x->leaf == 1) {
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i + 1] = x->keys[i];
            x->offsets[i + 1] = x->offsets[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->offsets[i + 1] = offset;
        x->n = x->n + 1;
    } else {
        while (i >= 0 && x->keys[i] > k)
            i--;
        if (x->C[i + 1]->n == 2 * x->t - 1) {
            splitChild(x, i + 1, x->C[i + 1]);
            if (x->keys[i + 1] < k)
                i++;
        }
        insertNonFull(x->C[i + 1], k, offset);
    }
}

void splitChild(BTreeNode *x, int i, BTreeNode *y) {
    BTreeNode *z = createNode(y->t, y->leaf);
    z->n = y->t - 1;
    for (int j = 0; j < y->t - 1; j++) {
        z->keys[j] = y->keys[j + y->t];
        z->offsets[j] = y->offsets[j + y->t];
    }
    if (y->leaf == 0) {
        for (int j = 0; j < y->t; j++)
            z->C[j] = y->C[j + y->t];
    }
    y->n = y->t - 1;
    for (int j = x->n; j >= i + 1; j--)
        x->C[j + 1] = x->C[j];
    x->C[i + 1] = z;
    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
        x->offsets[j + 1] = x->offsets[j];
    }
    x->keys[i] = y->keys[y->t - 1];
    x->offsets[i] = y->offsets[y->t - 1];
    x->n = x->n + 1;
}

void insert(BTreeNode **root, uint32_t k, long offset) {
    BTreeNode *r = *root;
    if (r->n == 2 * r->t - 1) {
        BTreeNode *s = createNode(r->t, 0);
        *root = s;
        s->C[0] = r;
        splitChild(s, 0, r);
        insertNonFull(s, k, offset);
    } else
        insertNonFull(r, k, offset);
}
BTreeNode *search(BTreeNode *root, uint32_t k) {
    int i = 0;
    while (i < root->n && k > root->keys[i])
        i++;
    if (i < root->n && root->keys[i] == k)
        return root;
    if (root->leaf == 1)
        return NULL;
    return search(root->C[i], k);
}
void writeCompressedDataToDisk(const char *filename, const char *data, uint32_t data_len) {
    FILE *file = fopen(filename, "ab");
    if (file != NULL) {
        gzFile gzfile = gzdopen(fileno(file), "wb");
        if (gzfile != NULL) {
            gzwrite(gzfile, data, data_len);
            gzclose(gzfile);
        }
        fclose(file);
    } else {
        perror("File opening failed");
    }
}

char *readCompressedDataFromDisk(const char *filename, uint32_t data_len) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("File opening failed");
        return NULL;
    }
    gzFile gzfile = gzdopen(fileno(file), "rb");
    if (gzfile == NULL) {
        fclose(file);
        return NULL;
    }
    char *data = (char *)malloc(data_len);
    gzread(gzfile, data, data_len);
    gzclose(gzfile);
    fclose(file);
    return data;
}

BPlusTreeNode *createBPlusNode(int t, int leaf) {
    BPlusTreeNode *node = (BPlusTreeNode *)malloc(sizeof(BPlusTreeNode));
    node->t = t;
    node->leaf = leaf;
    node->keys = (uint32_t *)malloc((2 * t - 1) * sizeof(uint32_t));
    node->values = (char **)malloc((2 * t - 1) * sizeof(char *));
    node->C = (BPlusTreeNode **)malloc(2 * t * sizeof(BPlusTreeNode *));
    node->n = 0;
    node->next = NULL;
    return node;
}
Database *createDatabase() {
    Database *db = (Database *)malloc(sizeof(Database));
    db->indexRoot = createNode(T, 1); // B樹根節點，用於索引
    db->dataRoot = createBPlusNode(T, 1); // B+樹根節點，用於存儲完整數據
    return db;
}

void insertToDatabase(Database *db, const char *key, const char *data) {
    uint32_t hashValue = hashFunction(key);
    long offset = ftell(fopen("shard_data.dat", "ab")); // 假設所有數據存儲在shard_data.dat中
    insert(&(db->indexRoot), hashValue, offset);

    writeCompressedDataToDisk("shard_data.dat", data, strlen(data));
}

char *searchInDatabase(Database *db, const char *key) {
    uint32_t hashValue = hashFunction(key);
    BTreeNode *node = search(db->indexRoot, hashValue);
    if (node == NULL) {
        return NULL;
    }
    long offset = node->offsets[0]; // 簡化處理，只考慮找到的第一個匹配項
    fseek(fopen("shard_data.dat", "rb"), offset, SEEK_SET);
    return readCompressedDataFromDisk("shard_data.dat", 256); // 假設數據不超過256字節
}

int main() {
    Database *db = createDatabase();
    insertToDatabase(db, "key1", "This is some data for key1");
    char* name = "key1";
    char *data = searchInDatabase(db, name);
    if (data) {
        printf("Data for key1: %s\n", data);
        free(data);
    } else {
        printf("Data not found for %s\n", name);
    }
    char* name1 = "key2";
    char *data1 = searchInDatabase(db, name1);
    if (data1) {
        printf("Data for key2: %s\n", data1);
        free(data);
    } else {
        printf("Data not found for %s\n", name1);
    }
    return 0;
}
