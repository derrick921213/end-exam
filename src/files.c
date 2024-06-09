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
    if (sscanf(line, "%31[^,],%d", parsed->id, &parsed->number) == 2)
    {
        return parsed;
    }
    else
    {
        free(parsed);
        return NULL;
    }
}
void ProcessFile(const char *filename, BPlusTreeNode **root)
{
    DataNode *data_list = NULL;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine *parsed = parse_line(line);
        if (parsed != NULL)
        {
            insert(root, parsed->id);
            DataNode_insert(&data_list, parsed->id, parsed->number);
        }
    }
    write_data_to_files(data_list);
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
void write_data_to_files(DataNode *head)
{
    DataNode *current = head;
    while (current)
    {
        char filename[256];
        sprintf(filename, "%s/%lu", STUDENT_COURSDE, current->hash_value);
        FILE *file = fopen(filename, "a"); // 使用追加模式
        if (!file)
        {
            printf("無法創建檔案 %s\n", filename);
            current = current->next;
        }
        fprintf(file, "%d\n", current->data->number); // 將課程ID寫入哈希文件
        fclose(file);
        current = current->next;
    }
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