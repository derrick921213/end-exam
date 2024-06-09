#include "LinkedList.h"
Node *create_list_node(int data)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}
void list_insert(Node **head, int data)
{
    Node *new_node = create_list_node(data);
    if (!new_node)
        return;
    new_node->next = *head;
    *head = new_node;
}
Node *list_search(Node *head, int data)
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->data == data)
            return current;
        current = current->next;
    }
    return NULL;
}
void list_delete(Node **head, int data)
{
    Node *temp = *head;
    Node *prev = NULL;
    if (temp != NULL && temp->data == data)
    {
        *head = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != data)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp);
}
void print_list(Node *head)
{
    Node *current = head;
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

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
void DataNode_write_files(DataNode *head,char *location)
{
    DataNode *current = head;
    while (current)
    {
        char filename[256];
        sprintf(filename, "%s/%lu", location, current->hash_value);
        FILE *file = fopen(filename, "a");
        if (!file)
        {
            printf("無法創建檔案 %s\n", filename);
            current = current->next;
        }
        fprintf(file, "%s\n", current->data->number);
        fclose(file);
        current = current->next;
    }
}