#include "ParseLine.h"
int split_index1 = 0;
int split_index2 = 0;
ParsedLine* parse_line(const char *line) {
    ParsedLine *result = (ParsedLine *)malloc(sizeof(ParsedLine));
    char id[32];
    int number;
    char name[256];
    if (sscanf(line, "%31[^,],%d,%255[^\n]", id, &number, name) != 3) {
        fprintf(stderr, "Failed to parse line: %s\n", line);
        return NULL;
    }

    result->id = (char*)malloc(sizeof(char)*(strlen(id)+1));
    strcpy(result->id, id);
    result->number = (char *)malloc(12); // To store the integer as string
    snprintf(result->number, 12, "%d", number);
    result->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(result->name, name);
    if (!result->id || !result->number || !result->name) {
        fprintf(stderr, "Memory allocation failed\n");
        if (result->id) free(result->id);
        if (result->number) free(result->number);
        if (result->name) free(result->name);
        return NULL;
    }

    return result;
}

void free_parse_line(ParsedLine *line)
{
    free(line->id);
    free(line->number);
    free(line->name);
}
void ProcessFile(const char *filename, BPlusTreeNode **root, BPlusTreeNode **root2)
{
    DataNode *data_list = NULL;
    DataNode *data_list2 = NULL;
    int first_line = 1;
    FILE *file = open_file(filename, "r");
    char line[500];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        if (first_line)
        {
            first_line = 0;
            continue;
        }
        ParsedLine *result = parse_line(line);
        if (result)
        {
            // printf("ID: %s, Number: %s, Name: %s\n", result->id, result->number, result->name);
            insert(root, result->id);
            insert(root2, result->number);
            DataNode_insert(&data_list, result->id, result->number, result->name);
            DataNode_insert(&data_list2, result->number, result->id, result->name);
        }
        // free_parse_line(result);
        free(result);
    }
    DataNode_write_files(data_list, STUDENT_COURSDE);
    DataNode_write_files(data_list2, COURSE_STUDENT);
    DataNode_write_index(data_list, STUDENT_TO_STUDENT_HASH, STUDENT_COURSDE, &split_index1);
    DataNode_write_index(data_list2, COURSE_TO_COURSE_HASH, COURSE_STUDENT, &split_index2);
    DataNode_free(data_list);
    DataNode_free(data_list2);
    close_file(file);
}