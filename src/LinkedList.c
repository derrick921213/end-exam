#include "LinkedList.h"

void DataNode_insert(DataNode **head, const char *student_id, char *course_id)
{
    unsigned long hash_value = hash_function(student_id);
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
    strcpy(new_node->data->number ,course_id);
    new_node->next = *head;
    *head = new_node;
}
void DataNode_free(DataNode *head)
{
    DataNode *current = head;
    while (current)
    {
        DataNode *next = current->next;
        free(current);
        current = next;
    }
}

void DataNode_write_files(DataNode *head, char *location) {
    DataNode *current = head;
    int Split_Index = 0;

    while (current) {
        char filename[256];
        char count[256];
        FILE *countfile = NULL;

        sprintf(filename, "%s/%lu", location, current->hash_value);
        sprintf(count, "%s/%lu/count", location, current->hash_value);

        if (!isDirectoryExists(filename)) {
            create_directory(filename);
        }

        if (!isFileExistsStats(count)) {
            Split_Index = 0;
            countfile = fopen(count, "wb+");
        } else {
            countfile = fopen(count, "rb+");
            fread(&Split_Index, sizeof(int), 1, countfile);
            fclose(countfile);
            countfile = fopen(count, "wb+");
            // printf("Split_Index: %d\n", Split_Index);
        }

        sprintf(filename, "%s/%lu/%d", location, current->hash_value, Split_Index);
        FILE *file = fopen(filename, "a");
        if (!file) {
            printf("Cannot create data %s\n", filename);
            exit(EXIT_FAILURE);
        }

        char dataToWrite[512];
        int dataLength = snprintf(dataToWrite, sizeof(dataToWrite), "%s\n", current->data->number);
        if (getFileSize(file) + dataLength > MAX_FILE_SIZE) {
            fclose(file);
            sprintf(filename, "%s/%lu/%d", location, current->hash_value, ++Split_Index);
            file = fopen(filename, "a");
            if (!file) {
                printf("Cannot create file %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }

        fprintf(file, "%s\n", current->data->number);
        fwrite(&Split_Index, sizeof(int), 1, countfile);
        fclose(file);
        fclose(countfile);

        current = current->next;
    }
}


void DataNode_write_index(DataNode *head, char *location, char *src, int *split_index) {
    DataNode *current = head;
    
    FILE *file = NULL;
    char filename[256];
    
    while (current) {
        if (!file) {
            sprintf(filename, "%s/%d", location, ++(*split_index));
            file = fopen(filename, "a");
            if (!file) {
                printf("無法創建檔案 %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }
        
        char dataToWrite[512];
        int dataLength = snprintf(dataToWrite, sizeof(dataToWrite), "%s %s/%lu\n", current->data->id, src, current->hash_value);
        
        if (getFileSize(file) + dataLength > MAX_FILE_SIZE) {
            fclose(file);
            sprintf(filename, "%s/%d", location, ++(*split_index));
            file = fopen(filename, "a");
            if (!file) {
                printf("無法創建檔案 %s\n", filename);
                exit(EXIT_FAILURE);
            }
        }

        fprintf(file, "%s", dataToWrite);
        current = current->next;
    }

    if (file) {
        fclose(file);
    }
}
