#include "main.h"
int FILECOUNT = 0;
BPlusTreeNode *Student_Course = NULL;
BPlusTreeNode *Course_Student = NULL;
char *cwd = NULL;
char *input = NULL;

void ManyFile(const int n)
{
    Student_Course = create_node(true);
    Course_Student = create_node(false);
    for (int i = 1; i <= n; i++)
    {
        printf("\r");
        printProgressBar(i, n);
        char filename[32];
        sprintf(filename, "DATA/%s/%04d", input,i);
        ProcessFile(filename, &Student_Course, &Course_Student);
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
        const char* to_create[] = {INDEXDIR, STUDENT_COURSDE,COURSE_STUDENT};
        int length = sizeof(to_create) / sizeof(to_create[0]);
        delete_create_dir(to_create, length);
        ManyFile(FILECOUNT);
    }
    else
    {
        fprintf(stderr, "Makefile not found\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int changeCWD(void)
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
        if (search(Student_Course, student_id))
        {
            printf("Student ID found\n");
        }
        else
        {
            printf("Student ID not found\n");
        }
    }
    free_node(Student_Course);
    return 0;
}
