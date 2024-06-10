#include "Utils.h"
void printColored(FILE *stream, const char *message, const char *color)
{
    fprintf(stream, "%s%s%s\n", color, message,COLOR_RESET);
}
void clearScreen(void)
{
    system(CLEAR);
}