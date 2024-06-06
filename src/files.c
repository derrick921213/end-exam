#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#define MAX_LINE_LENGTH 256

FILE* open_file(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", filename);
        exit(1);
    }
    return file;
}

void close_file(FILE* file) {
    fclose(file);
}

ParsedLine* parse_line(const char* line) {
    ParsedLine* parsed = (ParsedLine*)malloc(sizeof(ParsedLine));
    if (parsed == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory\n");
        exit(1);
    }

    if (sscanf(line, "%31[^,],%d", parsed->id, &parsed->number) == 2) {
        return parsed;
    } else {
        free(parsed);
        return NULL;
    }
}

void ProcessFile(const char* filename){
    FILE* file = open_file(filename, "r");
    char line[MAX_LINE_LENGTH];
    printf("Reading file: %s\n", filename);
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        ParsedLine* parsed = parse_line(line);
        if (parsed != NULL) {
            printf("ID: %s, Number: %d\n", parsed->id, parsed->number);
            free(parsed);
        } else {
            fprintf(stderr, "No match found in line: %s\n", line);
        }
    }

    close_file(file);
}

void ManyFile(const int n){
    for(int i=1;i<=n;i++){
        char* filename = (char*)malloc(32);
        sprintf(filename, "data/data_no_cname/%04d", i);
        ProcessFile(filename);
        free(filename);
    }
}



// int main() {
//     ManyFile(1);
//     return 0;
// }
