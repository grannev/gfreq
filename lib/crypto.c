/* huffman algo to compress data realisation */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "crypto.h"
#include "converter.h"
#include "skew_heap.h"
#include "digmath.h"
#include "const.h"


void build_frequency(
        unsigned long *frequency, 
        struct byte_rw *brw
) {
    unsigned long i;

    for (i = 0; i < brw->in_len; i++)
        frequency[brw_next(brw)]++;
}


void build_frequency_heap(
        struct skew_heap *heap,
        const unsigned long *frequency
) {
    short i;

    for (i = 0; i < ASCII_SIZE; i++)
        if (frequency[i] != 0)
            sh_insert(heap, i, frequency[i]);
}


void build_huffman_tree(
        struct huffman_tree *tree,
        struct skew_heap *heap
) {
    struct node_ht *lch, *rch, *parent;

    while (heap->size > 1) {
        sh_pop(heap, &lch);
        sh_pop(heap, &rch);

        parent = malloc(sizeof(struct node_ht));
        node_ht_init_empty(parent);
        parent->byte = '!';
        parent->weight = lch->weight + rch->weight;

        ht_insert(tree, lch, rch, parent);
        sh_insert_node(heap, parent);
    }

    sh_pop(heap, &tree->root);
    sh_free(heap);
}

void build_code_table(
        unsigned long *encrypt_table,
        unsigned int *codes_len,
        struct node_ht *tnode,
        unsigned long code,
        unsigned int len
) {
    unsigned long left_code;
    unsigned long right_code;

    if (tnode == NULL)
        return;

    if (tnode->left == NULL && tnode->right == NULL) {
        encrypt_table[tnode->byte] = code;
        codes_len[tnode->byte] = len == 0 ? 1 : len;
        return;
    }

    len++;

    left_code = code;
    add_bit(&left_code, 0);
    build_code_table(encrypt_table, codes_len, tnode->left, left_code, len);

    right_code = code;
    add_bit(&right_code, 1);
    build_code_table(encrypt_table, codes_len, tnode->right, right_code, len);
}


void build_encrypt_table(
        unsigned long *encrypt_table,
        unsigned int *codes_len,
        struct huffman_tree *tree
) {
    if (tree != NULL)
        build_code_table(encrypt_table, codes_len, tree->root, 0, 0);
}


void count_encoded_data(
        unsigned int *codes_len,
        unsigned long *frequency,
        unsigned long *bits,
        unsigned long *bytes
) {
    unsigned long i;

    *bits = 0;
    for (i = 0; i < ASCII_SIZE; i++)
        *bits += frequency[i] * codes_len[i];
    *bytes = (*bits + 7) / 8;
}


void encrypt(struct byte_rw *brw)
{
    struct skew_heap frequency_heap;
    struct huffman_tree code_tree;

    unsigned long encrypt_table[ASCII_SIZE];
    unsigned long frequency[ASCII_SIZE];
    unsigned int  codes_len[ASCII_SIZE];
    unsigned long i, bits_len, bytes_len;

    if (brw->in_len == 0) {
        return;
    }

    for (i = 0; i < ASCII_SIZE; i++) {
        encrypt_table[i] = 0;
        frequency[i] = 0;
        codes_len[i] = 0;
    }

    build_frequency(frequency, brw);
    rewind(brw->in);
    brw->in_eof = 0;
    
    sh_init(&frequency_heap);
    build_frequency_heap(&frequency_heap, frequency);

    ht_init(&code_tree);
    build_huffman_tree(&code_tree, &frequency_heap);

    build_encrypt_table(encrypt_table, codes_len, &code_tree);

    convert_table(brw, frequency);
    ht_free(&code_tree);
    count_encoded_data(codes_len ,frequency, &bits_len, &bytes_len);
    
    brw_put_ulong(brw, bits_len);
    brw_put_uch(brw, '\n');
    brw_put_ulong(brw, bytes_len);
    brw_put_uch(brw, '\n');

    convert_bits(brw, encrypt_table, codes_len);
}


void decrypt(struct byte_rw *brw)
{
    struct skew_heap frequency_heap;
    struct huffman_tree code_tree;

    unsigned long frequency[ASCII_SIZE];
    unsigned long bits_len, bytes_len;

    if (brw->in_len == 0)
        return;

    deconvert_table(brw, frequency);

    sh_init(&frequency_heap);
    build_frequency_heap(&frequency_heap, frequency);

    ht_init(&code_tree);
    build_huffman_tree(&code_tree, &frequency_heap);

    bits_len = brw_next_ulong(brw);
    brw_next(brw);
    bytes_len = brw_next_ulong(brw);
    brw_next(brw);

    deconvert_bits(brw, &code_tree, bits_len, bytes_len);
    ht_free(&code_tree);
}


void compress(const char *in_file)
{
    struct byte_rw brw;
    char *out_file;

    out_file = malloc(strlen(in_file) + 5);
    strcpy(out_file, in_file);
    strcat(out_file, ".ger");

    brw_init(&brw, in_file, out_file);
    encrypt(&brw);

    brw_close(&brw);
    free(out_file);
}

void decompress(const char *in_file)
{
    struct byte_rw brw;
    unsigned long in_len, out_len;
    char *out_file;

    in_len = strlen(in_file);
    out_len = in_len - 4;
    out_file = malloc(out_len + 1);
    strncpy(out_file, in_file, out_len);
    out_file[out_len] = '\0';

    brw_init(&brw, in_file, out_file);
    decrypt(&brw);

    brw_close(&brw);
    free(out_file);
}

