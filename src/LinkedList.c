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

void DataNode_write_files(DataNode *head,char *location,char *mode)
{
    DataNode *current = head;
    while (current)
    {
        char filename[256];
        sprintf(filename, "%s/%lu", location, current->hash_value);
        FILE *file = fopen(filename, mode);
        if (!file)
        {
            printf("無法創建檔案 %s\n", filename);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s\n", current->data->number);
        fclose(file);
        current = current->next;
    }
}

void DataNode_write_index(DataNode *head,char *location,char *src)
{
    DataNode *current = head;
    int index = 0;
    while (current)
    {
        char filename[256];
        sprintf(filename, "%s/%d", location, ++index);
        FILE *file = fopen(filename, "w");
        if (!file)
        {
            printf("無法創建檔案 %s\n", filename);
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%s %s/%lu", current->data->id,src,current->hash_value);
        fclose(file);
        current = current->next;
    }
}