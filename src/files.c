#include "files.h"

void printProgressBar(int current, int total)
{
    int percentage = (current * 100) / total;
    int width = 50;
    int pos = width * percentage / 100;
    printf("[");
    for (int i = 0; i < width; ++i)
    {
        if (i < pos)
        {
            printf("=");
        }
        else if (i == pos)
        {
            printf(">");
        }
        else
        {
            printf(" ");
        }
    }
    printf("] %d%%", percentage);
    fflush(stdout);
}
FILE *open_file(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        exit(1);
    }
    return file;
}
void close_file(FILE *file)
{
    fclose(file);
}
ParsedLine *parse_line(const char *line)
{
    ParsedLine *parsed = (ParsedLine *)malloc(sizeof(ParsedLine));
    if (parsed == NULL)
    {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(1);
    }
    int number = 0;
    if (sscanf(line, "%31[^,],%d", parsed->id, &number) == 2)
    {
        sprintf(parsed->number, "%d", number);
        return parsed;
    }
    else
    {
        free(parsed);
        return NULL;
    }
}
void ProcessFile(const char *filename, BPlusTreeNode **root,BPlusTreeNode **root2)
{
    DataNode *data_list = NULL;
    DataNode *data_list2 = NULL;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine *parsed = parse_line(line);
        if (parsed != NULL)
        {
            insert(root, parsed->id);
            insert(root2, parsed->number);
            DataNode_insert(&data_list, parsed->id, parsed->number);
            DataNode_insert(&data_list2, parsed->number, parsed->id);
        }
    }
    DataNode_write_files(data_list,STUDENT_COURSDE);
    DataNode_write_files(data_list2,COURSE_STUDENT);
    DataNode_free(data_list);
    close_file(file);
}
int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
int isFileExistsStats(const char *path)
{
    struct stat stats;
    if (stat(path, &stats) == 0)
    {
        return 1;
    }
    return 0;
}

int create_directory(const char *dir_name)
{
#ifdef _WIN32
    if (mkdir(dir_name) == 0)
    {
#else
    if (mkdir(dir_name, 0755) == 0)
    {
#endif
        return 0;
    }
    else
    {
        perror("Error creating directory");
        return 1;
    }
}
int delete_directory(const char *dir_name)
{
    int status;
#ifdef _WIN32
    char command[512];
    snprintf(command, sizeof(command), "rd /s /q \"%s\"", dir_name);
#else
    char command[512];
    snprintf(command, sizeof(command), "rm -rf \"%s\"", dir_name);
#endif
    status = system(command);
    return status;
}
bool delete_create_dir(const char **dir_name, int length)
{
    bool GO = false;
    int count = 0;
    while (count < length)
    {
        if (isDirectoryExists(dir_name[count]))
        {
            if (delete_directory(dir_name[count]) != 0)
            {
                perror("Error deleting directory");
                break;
            }
        }
        create_directory(dir_name[count]);
        GO = true;
        count++;
    }
    return GO;
}
long getFileSize(FILE *file) {
    long currentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, currentPos, SEEK_SET);
    return size;
}