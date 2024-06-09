#include "LinkedList.h"
Node *create_node(int data)
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
    Node *new_node = create_node(data);
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
void print_list(Node* head) {
    Node* current = head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}