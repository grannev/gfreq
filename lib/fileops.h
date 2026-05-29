#ifndef FILEOPS_H
#define FILEOPS_H

long get_file_size(const char *name);

/* buffer must be uninitialized and not allocated */
void read_from_file(struct ubuffer *in_buffer, const char *name);

void write_to_file(const char *name, const struct ubuffer *in_buffer);

#endif

