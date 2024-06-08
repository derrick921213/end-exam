#ifndef QUICK_SORT_H
#define QUICK_SORT_H
#include <stdio.h>
typedef struct Data {
    char id[256];
} Data;
int partition(Data *, int, int);
void quicksort(Data *, int, int);
void sort_and_write_data(Data *, int, const char *);
#endif