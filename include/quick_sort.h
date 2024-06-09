#ifndef QUICK_SORT_H
#define QUICK_SORT_H
#include "Utils.h"
int partition(Data *, int, int);
void quicksort(Data *, int, int);
void sort_and_write_data(Data *, int, const char *);
#endif