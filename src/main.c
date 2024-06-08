// #include "BPlusTree.h"
// #include "files.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #ifdef _WIN32
// #include <windows.h>
// #else
// #include <unistd.h>
// #include <limits.h>
// #endif
// #define FILECOUNT 466
// #define INDEXDIR "data/index"
// #define TREEDIR "data/trees"
// #define TREEFILE TREEDIR"/bplustree"
// #define DATA "data"

// BPlusTreeNode *root = NULL;
// char *cwd = NULL;

// void ManyFile(const int n)
// {
//     root = create_node(true);
//     for (int i = 1; i <= n; i++)
//     {
//         printf("\r");
//         printProgressBar(i, n);
//         char filename[32];
//         sprintf(filename, "data/data_no_cname/%04d", i);
//         ProcessFile(filename, &root);
//     }
//     printf("\n");
//     // print_tree(root, 0);
//     save_tree("data/bplustree.dat", root);
// }
// int initialization(void)
// {
//     printf("Initializing\n");
//     if (!isDirectoryExists(DATA))
//     {
//         return -1;
//     }
//     printf("Checking for Makefile\n");
//     if (isFileExistsStats("Makefile"))
//     {
//         printf("Makefile found\n");
//         printf("Checking for index directory\n");
//         if (!isDirectoryExists(INDEXDIR))
//         {
//             if (create_directory(INDEXDIR) == 0)
//             {
//                 ManyFile(FILECOUNT);
//             }
//             else
//             {
//                 perror("Error in creating directory");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             printf("check %s\n",TREEFILE);
//             if (isFileExistsStats(TREEFILE))
//             {
//                 printf("Loading B+ tree\n");
//                 root = load_tree(TREEFILE);
//                 if (!root)
//                 {
//                     fprintf(stderr, "Error in loading B+ tree\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 else
//                 {
//                     printf("B+ tree loaded successfully\n");
//                 }
//             }
//             else
//             {
//                 ManyFile(FILECOUNT);
//             }
//         }
//     }
//     else
//     {
//         fprintf(stderr, "Makefile not found\n");
//         exit(EXIT_FAILURE);
//     }
//     return 0;
// }
// int changeCWD()
// {
// #ifdef _WIN32
//     if (SetCurrentDirectory(cwd)==0)
//     {
//         fprintf(stderr, "SetCurrentDirectory() error: %ld\n", GetLastError());
//         return 1;
//     }
// #else
//     if (chdir(cwd) != 0)
//     {
//         perror("chdir() error");
//         return 1;
//     }
// #endif
//     return 0;
// }
// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
//         return 1;
//     }
//     cwd = argv[1];
//     changeCWD();

//     if (initialization() != 0)
//     {
//         fprintf(stderr, "Error in initialization\n");
//         return 1;
//     }
//     char student_id[100];
//     while (true)
//     {
//         printf("Enter Student ID to search (or 'exit' to quit): ");
//         scanf("%s", student_id);
//         if (strcmp(student_id, "exit") == 0)
//         {
//             break;
//         }
//         printf("Searching for Student ID: %s\n", student_id);
//         if (search(root, student_id))
//         {
//             printf("Student ID found\n");
//         }
//         else
//         {
//             printf("Student ID not found\n");
//         }
//     }
//     // print_tree(root, 0);
//     free_node(root);
//     return 0;
// }

#include "BPlusTree.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif
#define FILECOUNT 10
#define INDEXDIR "data/index"
#define TREEDIR "data/trees"
#define TREEFILE TREEDIR "/bplustree"
#define DATA "data"

BPlusTreeNode *root = NULL;
char *cwd = NULL;

void ManyFile(const int n)
{
    root = create_node(true);
    for (int i = 1; i <= n; i++)
    {
        printf("\r");
        printProgressBar(i, n);
        char filename[32];
        sprintf(filename, "data/data_no_cname/%04d", i);
        ProcessFile(filename, &root);
    }
    printf("\n");
}

int initialization(void)
{
    printf("Initializing\n");
    if (!isDirectoryExists(DATA))
    {
        return -1;
    }
    printf("Checking for Makefile\n");
    if (isFileExistsStats("Makefile"))
    {
        if (isDirectoryExists(INDEXDIR))
        {
            if (delete_directory(INDEXDIR) != 0)
            {
                perror("Error deleting directory");
                return 1;
            }
        }
        create_directory(INDEXDIR);
        ManyFile(FILECOUNT);
    }
    else
    {
        fprintf(stderr, "Makefile not found\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int changeCWD()
{
#ifdef _WIN32
    if (SetCurrentDirectory(cwd) == 0)
    {
        fprintf(stderr, "SetCurrentDirectory() error: %ld\n", GetLastError());
        return 1;
    }
#else
    if (chdir(cwd) != 0)
    {
        perror("chdir() error");
        return 1;
    }
#endif
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }
    cwd = argv[1];
    changeCWD();

    if (initialization() != 0)
    {
        fprintf(stderr, "Error in initialization\n");
        return 1;
    }
    char student_id[100];
    while (true)
    {
        printf("Enter Student ID to search (or 'exit' to quit): ");
        scanf("%s", student_id);
        if (strcmp(student_id, "exit") == 0)
        {
            break;
        }
        printf("Searching for Student ID: %s\n", student_id);
        if (search(root, student_id))
        {
            printf("Student ID found\n");
        }
        else
        {
            printf("Student ID not found\n");
        }
    }
    free_node(root);
    return 0;
}
