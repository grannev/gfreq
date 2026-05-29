#include <stdlib.h>
#include <stdio.h>

#include "ubuffer.h"
#include "fileops.h"

long get_file_size(const char *name)
{
    long size;
    FILE *file;

    file = fopen(name, "rb");
    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    return size;
}

void read_from_file(struct ubuffer *out_buffer, const char *name)
{
    long size;
    FILE *file;

    size = get_file_size(name);

    file = fopen(name, "rb");
    if (file == NULL)
        exit(1);

    out_buffer->buffer = malloc(size);
    fread(out_buffer->buffer, 1, size, file);
    out_buffer->len = size;
    out_buffer->cap = size;

    fclose(file);
}

void write_to_file(const char *name, const struct ubuffer *in_buffer)
{
    FILE *file;

    file = fopen(name, "wb");
    if (file == NULL)
        exit(1);

    fwrite(in_buffer->buffer, 1, in_buffer->len, file);
    fclose(file);
}
