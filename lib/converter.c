#include <stdio.h>
#include <stdlib.h>

#include "converter.h"
#include "const.h"

void convert_table(struct byte_rw *brw, unsigned long *table)
{
    unsigned long i, size;

    size = 0;
    for (i = 0; i < ASCII_SIZE; i++)
        if (table[i] != 0)
            size++;

    brw_put_ulong(brw, size);
    brw_put_uch(brw, '\n');

    for (i = 0; i < ASCII_SIZE; i++) {
        if (table[i] != 0) {
#ifdef BEAUTIFUL_TABLE
            if (i != '\n') {
                brw_put_uch(brw, '\'');
#endif
                brw_put_uch(brw, (unsigned char) i);
#ifdef BEAUTIFUL_TABLE
                brw_put_uch(brw, '\'');
            } else {
                brw_put_uch(brw, '\\');
                brw_put_uch(brw, 'n');
            }
            brw_put_uch(brw, ' ');
#endif
            brw_put_uch(brw, ' ');
            brw_put_ulong(brw, table[i]);
            brw_put_uch(brw, '\n');
        }
    }

#ifdef BEAUTIFUL_TABLE
    brw_put_uch(brw, '\n');
#endif
}

void deconvert_table(struct byte_rw *brw, unsigned long *frequency)
{
    unsigned long table_size, i;
    unsigned char ch;

    for (i = 0; i < ASCII_SIZE; i++)
        frequency[i] = 0;

    table_size = brw_next_ulong(brw);
    brw_next(brw);

    for (i = 0; i < table_size; i++) {
        ch = brw_next(brw);
        brw_next(brw);
        frequency[ch] = brw_next_ulong(brw);
        brw_next(brw);
    }
}


void convert_bits(
        struct byte_rw *brw,
        const unsigned long *encrypt_table,
        const unsigned int *codes_len
) {
    unsigned char in_byte, out_byte;
    unsigned long code;
    unsigned long i, j, bit_index, used_bits;

    rewind(brw->in);
    brw->in_eof = 0;

    out_byte = 0;
    used_bits = 0;

    for (i = 0; i < brw->in_len; i++) {
        in_byte = brw_next(brw);
        code = encrypt_table[in_byte];

        for (j = 0; j < codes_len[in_byte]; j++) {
            bit_index = codes_len[in_byte] - j - 1;
            out_byte <<= 1;
            out_byte |= (code >> bit_index) & 1;
            used_bits++;

            if (used_bits == 8) {
                brw_put_uch(brw, out_byte);
                out_byte = 0;
                used_bits = 0;
            }
        }
    }

    if (used_bits != 0) {
        out_byte <<= 8 - used_bits;
        brw_put_uch(brw, out_byte);
    }
}

void deconvert_bits(
        struct byte_rw *brw,
        const struct huffman_tree *tree,
        unsigned long bits_len,
        unsigned long bytes_len
) {
    const struct node_ht *tnode;
    unsigned long i;
    unsigned char byte;

    if (tree == NULL || tree->root == NULL)
        return;

    if (tree->root->left == NULL && tree->root->right == NULL) {
        for (i = 0; i < bits_len; i++)
            brw_put_uch(brw, tree->root->byte);

        for (i = 0; i < bytes_len; i++)
            brw_next(brw);

        return;
    }

    tnode = tree->root;
    byte = 0;

    for (i = 0; i < bits_len; i++) {
        if (i % 8 == 0)
            byte = brw_next(brw);

        if (byte & (1 << (7 - i % 8)))
            tnode = tnode->right;
        else
            tnode = tnode->left;

        if (tnode->left == NULL && tnode->right == NULL) {
            brw_put_uch(brw, tnode->byte);
            tnode = tree->root;
        }
    }
}
