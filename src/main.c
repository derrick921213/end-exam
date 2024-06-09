#include "main.h"
int FILECOUNT = 0;
BPlusTreeNode *root = NULL;
char *cwd = NULL;
char *input = NULL;

void ManyFile(const int n)
{
    root = create_node(true);
    for (int i = 1; i <= n; i++)
    {
        printf("\r");
        printProgressBar(i, n);
        char filename[32];
        sprintf(filename, "DATA/%s/%04d", input,i);
        ProcessFile(filename, &root);
    }
    printf("\n");
}

int initialization(void)
{
    if (!isDirectoryExists(DATA))
    {
        return -1;
    }
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
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <directory> <file_count> <input>\n", argv[0]);
        return 1;
    }
    cwd = argv[1];
    input = argv[3];
    FILECOUNT = atoi(argv[2]);
    changeCWD();

    if (initialization() != 0)
    {
        fprintf(stderr, "Error in initialization\n");
        return 1;
    }
    // print_tree(root, 0);
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
