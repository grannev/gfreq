/* haffman algo to compress data header */
#ifndef CRYPTO
#define CRYPTO

#include "skew_heap.h"

#define ASCII_SIZE 256

/* void init_frequency(long *frequency, const char *cstring); */

void encrypt(struct ubuffer *out_buffer, const struct ubuffer *in_buffer);

void decrypt(struct ubuffer *out_buffer, const struct ubuffer *in_buffer);

void archive(const char *out_file, const char *in_file);

#endif

