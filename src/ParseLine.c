#include "ParseLine.h"
int split_index1 = 0;
int split_index2 = 0;
int parse_line(const char *line, ParsedLine *result)
{
    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "^([^,]+),([^,]+),(.+)$";
    int reti;
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti)
    {
        fprintf(stderr, "Could not compile regex\n");
        return -1;
    }

    reti = regexec(&regex, line, 4, matches, 0);
    if (!reti)
    {
        int id_length = matches[1].rm_eo - matches[1].rm_so;
        int number_length = matches[2].rm_eo - matches[2].rm_so;
        int name_length = matches[3].rm_eo - matches[3].rm_so;
        result->id = (char *)malloc(sizeof(char) * (id_length + 1));
        result->number = (char *)malloc(sizeof(char) * (number_length + 1));
        result->name = (char *)malloc(sizeof(char) * (name_length + 1));

        if (!result->id || !result->number || !result->name)
        {
            fprintf(stderr, "Memory allocation failed\n");
            regfree(&regex);
            return -1;
        }
        strncpy(result->id, line + matches[1].rm_so, id_length);
        result->id[id_length + 1] = '\0';
        strncpy(result->number, line + matches[2].rm_so, number_length);
        result->number[number_length + 1] = '\0';
        strncpy(result->name, line + matches[3].rm_so, name_length);
        result->name[name_length + 1] = '\0';

        regfree(&regex);
        return 0;
    }
    else if (reti == REG_NOMATCH)
    {
        printf("No match found\n");
        regfree(&regex);
        return 0;
    }
    else
    {
        char msgbuf[100];
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
        return -1;
    }
}

void free_parse_line(ParsedLine *line)
{
    if (line->id)
    {
        free(line->id);
    }
    if (line->number)
    {
        free(line->number);
    }
    if (line->name)
    {
        free(line->name);
    }
}
void ProcessFile(const char *filename, BPlusTreeNode **root, BPlusTreeNode **root2)
{
    DataNode *data_list = NULL;
    DataNode *data_list2 = NULL;
    int first_line = 1;
    FILE *file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    ParsedLine *result = (ParsedLine *)malloc(sizeof(ParsedLine));
    if (!result)
    {
        perror("Failed to allocate memory for ParsedLine");
        free(result);
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        if (first_line)
        {

            first_line = 0;
            continue; // 跳過第一行
        }

        if (parse_line(line, result) == 0)
        {
            // printf("id: %s, number: %s, name: %s\n", result->id, result->number, result->name);
            // printf("id: %s, number: %s\n", result->id, result->number);
            insert(root, result->id);
            insert(root2, result->number);
            DataNode_insert(&data_list, result->id, result->number, result->name);
            DataNode_insert(&data_list2, result->number, result->id, result->name);
        }
    }
    DataNode_write_files(data_list,STUDENT_COURSDE);
    DataNode_write_files(data_list2,COURSE_STUDENT);
    DataNode_write_index(data_list,STUDENT_TO_STUDENT_HASH,STUDENT_COURSDE,&split_index1);
    DataNode_write_index(data_list2,COURSE_TO_COURSE_HASH,COURSE_STUDENT,&split_index2);
    DataNode_free(data_list);
    DataNode_free(data_list2);
    free_parse_line(result);
    close_file(file);
}