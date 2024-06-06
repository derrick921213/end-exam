#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "files.h"
#include "BPlusTree.h"
#include "hashes.h"
#define MAX_LINE_LENGTH 256

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

    if (sscanf(line, "%31[^,],%31s", parsed->id, parsed->number) == 2)
    {
        return parsed;
    }
    else
    {
        free(parsed);
        return NULL;
    }
}

void ProcessFile(const char *filename, BPlusTreeNode *root, BPlusTreeNode *root2)
{
    DataNode *data_list = NULL;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    printf("Reading file: %s\n", filename);
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine *parsed = parse_line(line);
        if (parsed != NULL)
        {
            insert(&root, parsed->id);
            insert(&root2, parsed->number);
            // printf("ID: %s, Number: %d\n", parsed->id, parsed->number);
            store_data(&data_list, hash_function(parsed->id), parsed->id,hash_function(parsed->number), parsed->number);
        }
        // else
        // {
        //     fprintf(stderr, "No match found in line: %s\n", line);
        // }
    }
    write_data_to_files(data_list);
    // print_tree(root, 0);
    // print_tree(root2, 0);
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
        char filename1[256], filename2[256];
        sprintf(filename1, "data/index/student_to_course/%lu", current->student_hash_value);
        sprintf(filename2, "data/index/course_to_student/%lu", current->course_hash_value);
        
        FILE *file1 = fopen(filename1, "a");
        FILE *file2 = fopen(filename2, "a");
        if (!file1 || !file2)
        {
            if (!file1) printf("無法創建檔案 %s\n", filename1);
            if (!file2) printf("無法創建檔案 %s\n", filename2);
            if (file1) fclose(file1);
            if (file2) fclose(file2);
            current = current->next;
            continue;
        }

        fprintf(file1, "%s\n", current->data->number);
        fprintf(file2, "%s\n", current->data->id);

        fclose(file1);
        fclose(file2);

        current = current->next;
    }
}

void store_data(DataNode **head, unsigned long student_hash_value, const char *student_id,unsigned long course_hash_value, const char *course_id)
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