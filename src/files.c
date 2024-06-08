#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir _mkdir
#define rmdir _rmdir
#else
#include <unistd.h>
#include <limits.h>
#endif
#include "files.h"
#include "BPlusTree.h"
#include "hashes.h"
#define MAX_LINE_LENGTH 256

void printProgressBar(int current, int total) {
    int percentage = (current * 100) / total;
    int width = 50; // 進度條的寬度
    int pos = width * percentage / 100;
    
    printf("[");
    for (int i = 0; i < width; ++i) {
        if (i < pos) {
            printf("=");
        } else if (i == pos) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%", percentage);
    fflush(stdout);
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
    ParsedLine *parsed = (ParsedLine *)malloc(sizeof(ParsedLine));
    if (parsed == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(1);
    }

    if (sscanf(line, "%31[^,],%d", parsed->id, &parsed->number) == 2)
    {
        return parsed;
    }
    else
    {
        free(parsed);
        return NULL;
    }
}

void ProcessFile(const char *filename, BPlusTreeNode **root)
{
    DataNode *data_list = NULL;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    // printf("Reading file: %s\n", filename);
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine *parsed = parse_line(line);
        if (parsed != NULL)
        {
            insert(root, parsed->id);
            // printf("ID: %s, Number: %d\n", parsed->id, parsed->number);
            store_data(&data_list, hash_function(parsed->id), parsed->id, parsed->number);
        }
    }
    write_data_to_files(data_list);
    free_data_list(data_list);
    close_file(file);
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

void write_data_to_files(DataNode *head)
{
    DataNode *current = head;
    while (current)
    {
        char filename[256];
        sprintf(filename, "data/index/%lu_hash", current->hash_value);
        FILE *file = fopen(filename, "a"); // 使用追加模式
        if (!file)
        {
            printf("無法創建檔案 %s\n", filename);
            current = current->next;
        }
        fprintf(file, "%d\n", current->data->number); // 將課程ID寫入哈希文件
        fclose(file);
        current = current->next;
    }
}

void store_data(DataNode **head, unsigned long hash_value, const char *student_id, int course_id)
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
    new_node->hash_value = hash_value;
    strcpy(new_node->data->id, student_id);
    new_node->data->number = course_id;
    new_node->next = *head;
    *head = new_node;
}
int create_directory(const char* dir_name) {
    if (mkdir(dir_name, 0755) == 0) {
        return 0;
    } else {
        return 1;
    }
}
int delete_directory(const char* dir_name) {
    if (rmdir(dir_name) == 0) {
        printf("Directory '%s' deleted successfully.\n", dir_name);
        return 0; // Success
    } else {
        perror("Error deleting directory");
        return 1; // Failure
    }
}