#include "outputfile.h"
#define MAX_LINE_LENGTH 256
#define MAX_FILE_SIZE 2024

BPlusTreeNode *root = NULL;
BPlusTreeNode *root2 = NULL;

void ManyFile(const int n)
{
    root = create_node(true);
    root2 = create_node(true);
    for (int i = 1; i <= n; i++)
    {
        char *filename = (char *)malloc(32);
        sprintf(filename, "data/data_no_cname/%04d", i);
        ProcessFile(filename, root, root2);
        free(filename);
    }
}

FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        exit(1);
    }
    return file;
}

void close_file(FILE *file)
{
    fclose(file);
}

ParsedLine *parse_line(const char *line)
{
    int temp=0;
    ParsedLine *parsed = (ParsedLine *)malloc(sizeof(ParsedLine));
    if (parsed == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(1);
    }

    if (sscanf(line, "%31[^,],%d", parsed->id, &temp) == 2)
    {
        sprintf(parsed->number,"%d",temp);
        // printf("ID: %s, Number: %s\n", parsed->id, parsed->number);
        return parsed;
    }
    else
    {
        free(parsed);
        return NULL;
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

int isFileExistsStats(const char *path)
{   
    struct stat stats;
    if (stat(path, &stats) == 0)
    {
        return 1;
    }
    return 0;
}

void free_data_list(DataNode *head)
{
    DataNode *current = head;
    while (current)
    {
        DataNode *next = current->next;
        free(current);
        current = next;
    }
}

void ProcessFile(const char *filename, BPlusTreeNode *root, BPlusTreeNode *root2)
{
    DataNode *data_list = NULL;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    printf("Reading file: %s\n", filename);
    // if (fgets(line, sizeof(line), file) == NULL)
    // {
    //     fprintf(stderr, "Error: File is empty\n");
    //     exit(1);
    // }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine *parsed = parse_line(line);
        if (parsed != NULL)
        {
            insert(&root, parsed->id);
            insert(&root2, parsed->number);
            // printf("ID: %s, Number: %d\n", parsed->id, parsed->number);
            store_data(&data_list, hash_function(parsed->id), parsed->id, hash_function(parsed->number), parsed->number);
        }
        // else
        // {
        //     fprintf(stderr, "No match found in line: %s\n", line);
        // }
    }
    write_data_to_files(data_list,root,root2);
    // print_tree(root, 0);
    // print_tree(root2, 0);
    free_data_list(data_list);
    close_file(file);
}

int file_separate(char *filename_format, int *file_index, FILE **file, unsigned long hash_value, const char *id) {
    char filename[256];
    struct stat st;

    while (1) {
        if (id) {
            sprintf(filename, filename_format, id, *file_index);
        } else {
            sprintf(filename, filename_format, hash_value, *file_index);
        }

        if (stat(filename, &st) == 0 && st.st_size > MAX_FILE_SIZE) {
            (*file_index)++;
            continue;
        }
        *file = fopen(filename, "a");
        if (!*file) {
            printf("無法創建檔案 %s\n", filename);
            return -1;
        }
        return 0;
    }
}

void write_data_to_files(DataNode *head, BPlusTreeNode *root, BPlusTreeNode *root2) {
    DataNode *current = head;
    while (current) {
        int file_index1 = 0, file_index2 = 0, file_index3 = 0, file_index4 = 0;
        FILE *file1 = NULL, *file2 = NULL, *file3 = NULL, *file4 = NULL;

        int ret=0,ret2=0,ret3=0,ret4=0;
        ret = file_separate("data/index/student_to_student_hash/%s_%d", &file_index1, &file1, 0, current->data->id);
        ret2 = file_separate("data/index/course_to_course_hash/%s_%d", &file_index2, &file2, 0, current->data->number);
        ret3 = file_separate("data/index/student_hash_to_course/%lu_%d", &file_index3, &file3, current->student_hash_value, NULL);
        ret4 = file_separate("data/index/course_hash_to_student/%lu_%d", &file_index4, &file4, current->course_hash_value, NULL);
        if(ret && ret2 && ret3 && ret4){
            break;
        }

        if (!file1 || !file2 || !file3 && !file4) {
            if (file1) fclose(file1);
            if (file2) fclose(file2);
            if (file3) fclose(file3);
            if (file4) fclose(file4);
            current = current->next;
            continue;
        }

        if(search(root,current->data->id)){
            fprintf(file1, "%s data/index/student_hash_to_course/%lu\n", current->data->id, current->student_hash_value);
        }
        if (search(root2, current->data->number))
        {
            fprintf(file2, "%s data/index/course_hash_to_student/%lu\n", current->data->number, current->course_hash_value);
        }
        fprintf(file3,"%s\n", current->data->id);
        fprintf(file4, "%s\n", current->data->number);

        fclose(file1);
        fclose(file2);
        fclose(file3);
        fclose(file4);

        current = current->next;
    }
}

void store_data(DataNode **head, unsigned long student_hash_value, const char *student_id, unsigned long course_hash_value, const char *course_id)
{
    DataNode *new_node = (DataNode *)malloc(sizeof(DataNode));
    if (!new_node)
    {
        perror("Failed to allocate memory for DataNode");
        exit(EXIT_FAILURE);
    }
    new_node->data = (ParsedLine *)malloc(sizeof(ParsedLine));
    if (!new_node->data)
    {
        perror("Failed to allocate memory for ParsedLine");
        free(new_node);
        exit(EXIT_FAILURE);
    }
    new_node->student_hash_value = student_hash_value;
    new_node->course_hash_value = course_hash_value;
    strcpy(new_node->data->id, student_id);
    strcpy(new_node->data->number, course_id);
    new_node->next = *head;
    *head = new_node;
}

void write_data_to_screen(BPlusTreeNode *root, BPlusTreeNode *root2)
{
    while (true)
    {
        printf("Which do you want to search? 1.Student_Id 2.Course_Id 3.Exit\n");
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        if (choice == 1)
        {
            char student_id[100] = "";
            printf("Enter Student ID to search: ");
            scanf("%s", student_id);
            printf("Searching for Student ID: %s\n", student_id);
            if (search(root, student_id))
            {
                printf("Student ID found\n");
                unsigned long hash_value = hash_function(student_id);
                char filename[256];
                int file_index = 0;
                int data_count = 0;

                Data *data = (Data *)malloc(sizeof(Data) * 1024); // 初始分配
                int capacity = 1024;

                while (1)
                {
                    sprintf(filename, "data/index/student_to_course/%lu_%d", hash_value, file_index);
                    FILE *file = fopen(filename, "r");
                    if (file == NULL)
                    {
                        if (file_index == 0)
                            printf("Failed to open file: %s\n", filename);
                        break;
                    }

                    char line[256];
                    while (fscanf(file, "%s", line) != EOF)
                    {
                        if (data_count >= capacity)
                        {
                            capacity *= 2;
                            data = (Data *)realloc(data, sizeof(Data) * capacity);
                        }
                        strcpy(data[data_count].id, line);
                        data_count++;
                    }
                    fclose(file);
                    file_index++;
                }

                sort_and_write_data(data, data_count, "student");
                free(data);
            }
            else
            {
                printf("Student ID not found\n");
            }
        }
        else if (choice == 2)
        {
            char course_id[100] = "";
            printf("Enter Course ID to search: ");
            scanf("%s", course_id);
            printf("Searching for Course ID: %s\n", course_id);
            if (search(root2, course_id))
            {
                printf("Course ID found\n");
                unsigned long hash_value = hash_function(course_id);
                char filename[256];
                int file_index = 0;
                int data_count = 0;

                Data *data = (Data *)malloc(sizeof(Data) * 1024); // 初始分配
                int capacity = 1024;

                while (1)
                {
                    sprintf(filename, "data/index/course_to_student/%lu_%d", hash_value, file_index);
                    FILE *file = fopen(filename, "r");
                    if (file == NULL)
                    {
                        if (file_index == 0)
                            printf("Failed to open file: %s\n", filename);
                        break;
                    }

                    char line[256];
                    while (fscanf(file, "%s", line) != EOF)
                    {
                        if (data_count >= capacity)
                        {
                            capacity *= 2;
                            data = (Data *)realloc(data, sizeof(Data) * capacity);
                        }
                        strcpy(data[data_count].id, line);
                        data_count++;
                    }
                    fclose(file);
                    file_index++;
                }

                sort_and_write_data(data, data_count, "course");
                free(data);
            }
            else
            {
                printf("Course ID not found\n");
            }
        }
        else if (choice == 3)
        {
            break;
        }
        else
        {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }
}

//B+Tree
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

//Hash
unsigned long hash_function(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

//Quick Sort
// 快速排序的分區函數
int partition(Data *data, int low, int high)
{
    Data pivot = data[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (strlen(data[j].id) < strlen(pivot.id) || (strlen(data[j].id) == strlen(pivot.id) && strcmp(data[j].id, pivot.id) < 0))
        {
            i++;
            Data temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    Data temp = data[i + 1];
    data[i + 1] = data[high];
    data[high] = temp;

    return i + 1;
}
// 快速排序算法
void quicksort(Data *data, int low, int high)
{
    if (low < high)
    {
        int pi = partition(data, low, high);
        quicksort(data, low, pi - 1);
        quicksort(data, pi + 1, high);
    }
}

// 將排序後的數據寫入文件
void sort_and_write_data(Data *data, int count, const char *output_filename)
{
    quicksort(data, 0, count - 1);
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open output file '%s'\n", output_filename);
        exit(1);
    }
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", data[i].id);
        fprintf(output_file, "%s\n", data[i].id);
    }
    printf("Total count: %d\n", count);
    fprintf(output_file, "Total count: %d\n", count);
    fclose(output_file);
}

int main(){
    system("rmdir /S /Q data\\index\\student_to_student_hash");
    system("rmdir /S /Q data\\index\\course_to_course_hash");
    system("rmdir /S /Q data\\index\\student_hash_to_course");
    system("rmdir /S /Q data\\index\\course_hash_to_student");
    system("mkdir data\\index\\student_to_student_hash");
    system("mkdir data\\index\\course_to_course_hash");
    system("mkdir data\\index\\student_hash_to_course");
    system("mkdir data\\index\\course_hash_to_student");
    ManyFile(1);
    //write_data_to_screen(root, root2);
    return 0;
}