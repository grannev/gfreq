#include <stdio.h>

#include "fileops.h"

unsigned long get_file_size(const char *name)
{
    long size;
    FILE *file;

    file = fopen(name, "rb");
    if (file == NULL)
        return 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    return size;
}

