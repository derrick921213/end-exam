#include "BPlusTree.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
BPlusTreeNode *root = NULL;

void ManyFile(const int n)
{
    root = create_node(true);
    for (int i = 1; i <= n; i++)
    {
        char *filename = (char *)malloc(32);
        sprintf(filename, "data/data_no_cname/%04d", i);
        ProcessFile(filename, root);
        free(filename);
    }
}

int main(void)
{
    if (isFileExistsStats("Makefile"))
    {
        if (!isDirectoryExists("index"))
        {
            if (!system("mkdir index"))
            {
                ManyFile(466);
            }
            else
            {
                printf("Error in creating directory\n");
                exit(1);    
            }
        }
    }
    else
    {
        printf("Makefile not found\n");
        exit(1);
    }
    char student_id[100] = "";
    printf("Enter Student ID to search: ");
    scanf("%s", student_id);
    printf("Searching for Student ID: %s\n", student_id);
    if (search(root, student_id))
    {
        printf("Student ID found\n");
    }
    else
    {
        printf("Student ID not found\n");
    }
    free_node(root);
    return 0;
}