#ifndef BIT_BUFFER_H
#define BIT_BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#include "ubuffer.h"

#define ASCII_SIZE 256

struct bit_writer {
    unsigned char *bits;
    unsigned int byte_len;
    unsigned int bit_len;
};

void init_bw(struct bit_writer *writer, const struct ubuffer *bits);

void free_bw(struct bit_writer *writer);

/* 
 * 4 a2 00b2 01c2 10d2 11
 * 10 bbabefueanufbw
*/

void convert_table(struct ubuffer *out_buffer, const struct ubuffer *table);
void deconvert_table(
        struct ubuffer *table,
        const struct ubuffer *in_buffer,
        unsigned long *cursor);

void convert_bits(struct ubuffer *out_buffer, struct bit_writer *writer);
void deconvert_bits(
        struct ubuffer *ubuf,
        const struct ubuffer *in_buffer,
        unsigned long *cursor);

#endif

