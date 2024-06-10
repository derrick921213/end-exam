#include "main.h"
int FILECOUNT = 0;
BPlusTreeNode *Student_Course = NULL;
BPlusTreeNode *Course_Student = NULL;
char *cwd = NULL;
char *input = NULL;

void ManyFile(const int n)
{
    Student_Course = create_node(true);
    Course_Student = create_node(true);
    for (int i = 1; i <= n; i++)
    {
        printf("\r");
        printProgressBar(i, n);
        char filename[32];
        sprintf(filename, "DATA/%s/%04d", input, i);
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
        const char *to_create[] = {INDEXDIR,STUDENT_TO_STUDENT_HASH,COURSE_TO_COURSE_HASH, STUDENT_COURSDE, COURSE_STUDENT};
        int length = sizeof(to_create) / sizeof(to_create[0]);
        delete_create_dir(to_create, length);
        ManyFile(FILECOUNT);
    }
    else
    {
        printColored(stderr, "Makefile not found", RED);
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
    char message[256];
    if (argc != 4)
    {
        sprintf(message, "Usage: %s <directory> <number of files> <input>\n", argv[0]);
        printColored(stderr, message, RED);
        return 1;
    }
    cwd = argv[1];
    input = argv[3];
    FILECOUNT = atoi(argv[2]);
    changeCWD();

    if (initialization() != 0)
    {
        printColored(stderr, "Error in initialization\n", RED);
        return 1;
    }
    // print_tree(root, 0);
    char id[100];
    printf(PLACEHOLDER);
    int choose = 0;
    while (scanf("%d", &choose) && choose)
    {
        while (getchar() != '\n')
            continue;
        if (choose == 1)
        {
            printColored(stdout,"Enter Student ID to search (or 'exit' to quit): ",YEL);
            scanf("%s", id);
            if (strcmp(id, "exit") == 0)
            {
                break;
            }
            sprintf(message, "Searching for Student ID: %s\n", id);
            printColored(stdout,message,CYN);
            if (search(Student_Course, id))
            {
                printColored(stdout,"Student ID found\n",GRN);
                Write_terminal(STUDENT_COURSDE,id);
            }
            else
            {
                printColored(stdout,"Student ID not found\n",RED);
            }
        }
        else if (choose == 2)
        {
            printColored(stdout,"Enter Course ID to search (or 'exit' to quit): ",YEL);
            scanf("%s", id);
            if (strcmp(id, "exit") == 0)
            {
                break;
            }
            sprintf(message, "Searching for Course ID: %s\n", id);
            printColored(stdout,message,CYN);
            if (search(Course_Student, id))
            {
                printColored(stdout,"Course ID found\n",GRN);
                Write_terminal(COURSE_STUDENT,id);
            }
            else
            {
                printColored(stdout,"Course ID not found\n",RED);
            }
        }
        printf(PLACEHOLDER);
    }
    free_node(Student_Course);
    return 0;
}
