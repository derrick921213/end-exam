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
                ManyFile(10);
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
                    ManyFile(10);
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
                sprintf(filename, "data/index/student_to_course/%lu", hash_value);
                FILE *file = fopen(filename, "r");
                if (file == NULL)
                {
                    printf("Failed to open file: %s\n", filename);
                    continue;
                }
                char line[256];
                int i = 0;
                printf("Courses_id: \n");
                while (fscanf(file, "%s", line) != EOF)
                {
                    i++;
                    printf("%s\n", line);
                }
                printf("Number of courses: %d\n", i);
                fclose(file);
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
                sprintf(filename, "data/index/course_to_student/%lu", hash_value);
                FILE *file = fopen(filename, "r");
                if (file == NULL)
                {
                    printf("Failed to open file: %s\n", filename);
                    continue;
                }
                char line[256];
                printf("Student_id: \n");
                int i = 0;
                while (fscanf(file, "%s", line) != EOF)
                {
                    i++;
                    printf("%s\n", line);
                }
                printf("Number of students: %d\n", i);
                fclose(file);
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
    free_node(root);
    free_node(root2);
    return 0;
}