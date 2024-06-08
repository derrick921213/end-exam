#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "files.h"
#include "BPlusTree.h"
#include "hashes.h"
#define MAX_LINE_LENGTH 256

#define MAX_FILE_SIZE 2038

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
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fprintf(stderr, "Error: File is empty\n");
        exit(1);
    }
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
        int file_index1 = 0, file_index2 = 0;
        char filename1[256], filename2[256];
        FILE *file1 = NULL, *file2 = NULL;

        while (1)
        {
            sprintf(filename1, "data/index/student_to_course/%lu_%d", current->student_hash_value, file_index1);
            file1 = fopen(filename1, "a");
            if (!file1)
            {
                printf("無法創建檔案 %s\n", filename1);
                break;
            }

            fseek(file1, 0, SEEK_END);
            if (ftell(file1) < MAX_FILE_SIZE)
            {
                break;
            }

            fclose(file1);
            file1 = NULL;
            file_index1++;
        }

        while (1)
        {
            sprintf(filename2, "data/index/course_to_student/%lu_%d", current->course_hash_value, file_index2);
            file2 = fopen(filename2, "a");
            if (!file2)
            {
                printf("無法創建檔案 %s\n", filename2);
                break;
            }

            fseek(file2, 0, SEEK_END);
            if (ftell(file2) < MAX_FILE_SIZE)
            {
                break;
            }

            fclose(file2);
            file2 = NULL;
            file_index2++;
        }
        if (!file1 || !file2)
        {
            if (!file1)
                printf("無法創建檔案 %s\n", filename1);
            if (!file2)
                printf("無法創建檔案 %s\n", filename2);
            if (file1)
                fclose(file1);
            if (file2)
                fclose(file2);
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

                sort_and_write_data(data, data_count, "sorted_student_ids.txt");
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

                sort_and_write_data(data, data_count, "sorted_course_ids.txt");
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

// 对数据进行排序并写入文件，同时输出到终端
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