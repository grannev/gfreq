#ifndef BIT_BUFFER_H
#define BIT_BUFFER_H

#include "byte_rw.h"
#include "huffman.h"

/* convert_table is write frequency table in file using byte_rw struct */
void convert_table(struct byte_rw *brw, unsigned long *frequency);

/* deconvert_table is reading frequency table from file using byte_rw struct */
void deconvert_table(struct byte_rw *brw, unsigned long *frequency);

/* convert_bits is compressing bits to bytes and write them into file */
void convert_bits(
        struct byte_rw *brw,
        const unsigned long *encrypt_table,
        const unsigned int *codes_len);

/* deconvert_bits is reading bytes from file in decompress them into bits */
void deconvert_bits(
        struct byte_rw *brw,
        const struct huffman_tree *tree,
        unsigned long bits_len,
        unsigned long bytes_len);

#endif
