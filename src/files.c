#include "files.h"
int split_index1 = 0;
int split_index2 = 0;

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
    printf("] %d%% ", percentage);
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
    DataNode_write_index(data_list,STUDENT_TO_STUDENT_HASH,STUDENT_COURSDE,&split_index1);
    DataNode_write_index(data_list2,COURSE_TO_COURSE_HASH,COURSE_STUDENT,&split_index2);
    DataNode_free(data_list);
    DataNode_free(data_list2);
    close_file(file);
}

void Write_terminal(const char *filename,char *id){
    unsigned long hash_value = hash_function(id);
    int Terminal_INDEX = 0;
    int Now_Index = 0;
    char countfile[256];
    sprintf(countfile, "%s/%lu/count", filename, hash_value);

    FILE *count = open_file(countfile, "rb");
    fread(&Terminal_INDEX, sizeof(int), 1, count);
    fclose(count);

    Data *data = NULL;
    int data_count = 0;
    int capacity = 10; // Initial capacity
    data = (Data *)malloc(sizeof(Data) * capacity);

    while (Terminal_INDEX >= Now_Index) {
        char datafile[256];
        sprintf(datafile, "%s/%lu/%d", filename, hash_value, Now_Index);
        FILE *file = open_file(datafile, "r");
        char line[MAX_LINE_LENGTH];
        while (fscanf(file, "%s", line) != EOF) {
            if (data_count >= capacity) {
                capacity *= 2;
                data = (Data *)realloc(data, sizeof(Data) * capacity);
                if (!data) {
                    printf("Memory reallocation error\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(data[data_count].id, line);
            data_count++;
        }
        fclose(file);
        Now_Index++;
    }
    char message[256];
    sprintf(message, "Student ID Hash_Value :%lu", hash_value);
    // printf("Student ID Hash_Value :%lu", hash_value);
    printColored(stdout, message, MAG);
    sort_and_write_data(data, data_count, ANSWER);
    free(data);
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