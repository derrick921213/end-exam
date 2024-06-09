#include "Utils.h"
void printColored(FILE *stream, const char *message, const char *color)
{
    fprintf(stream, "%s%s\033[0m\n", color, message);
}
void clearScreen(void)
{
    system(CLEAR);
}