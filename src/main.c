#include "BPlusTree.h"
#include "hashes.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

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

int main(void)
{

    if (isFileExistsStats("Makefile"))
    {
        if (!isDirectoryExists("data/index"))
        {
            if (!system("mkdir data\\index"))
            {
                if (system("mkdir data\\index\\course_to_student") != 0)
                {
                    printf("Error in creating directory: data/index/student\n");
                    exit(1);
                }
                if (system("mkdir data\\index\\student_to_course") != 0)
                {
                    printf("Error in creating directory: data/index/course\n");
                    exit(1);
                }
                ManyFile(466);
            }
            else
            {
                printf("Error in creating directory\n");
                exit(1);
            }
        }
        else
        {
            system("rmdir /S /Q data\\index");
            if (!isDirectoryExists("data/index"))
            {
                if (!system("mkdir data\\index"))
                {
                    if (system("mkdir data\\index\\course_to_student") != 0)
                    {
                        printf("Error in creating directory: data/index/student\n");
                        exit(1);
                    }
                    if (system("mkdir data\\index\\student_to_course") != 0)
                    {
                        printf("Error in creating directory: data/index/course\n");
                        exit(1);
                    }
                    ManyFile(466);
                }
                else
                {
                    printf("Error in creating directory\n");
                    exit(1);
                }
            }
        }
    }
    else
    {
        printf("Makefile not found\n");
        exit(1);
    }

    write_data_to_screen(root, root2);

    free_node(root);
    free_node(root2);
    return 0;
}