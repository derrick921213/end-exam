#include "quick_sort.h"

// 快速排序的分區函數
int partition(Data *data, int low, int high)
{
    Data pivot = data[high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (strlen(data[j].id) < strlen(pivot.id) || (strlen(data[j].id) == strlen(pivot.id) && strcmp(data[j].id, pivot.id) < 0))
        {
            i++;
            Data temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    Data temp = data[i + 1];
    data[i + 1] = data[high];
    data[high] = temp;

    return i + 1;
}
// 快速排序算法
void quicksort(Data *data, int low, int high)
{
    if (low < high)
    {
        int pi = partition(data, low, high);
        quicksort(data, low, pi - 1);
        quicksort(data, pi + 1, high);
    }
}

// 对数据进行排序并写入文件，同时输出到终端
void sort_and_write_data(Data *data, int count, const char *output_filename)
{
    quicksort(data, 0, count - 1);
    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error: Unable to open output file '%s'\n", output_filename);
        exit(1);
    }
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", data[i].id);
        fprintf(output_file, "%s\n", data[i].id);
    }
    printf("Total count: %d\n", count);
    fprintf(output_file, "Total count: %d\n", count);
    fclose(output_file);
}