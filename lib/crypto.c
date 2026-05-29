/* huffman algo to compress data realisation */
#include <stdlib.h>
#include <stdio.h>

#include "crypto.h"
#include "bit_buffer.h"
#include "fileops.h"


void build_frequency(
        unsigned long *frequency, 
        const struct ubuffer *in_buffer
) {
    unsigned long i;

    for (i = 0; i < ASCII_SIZE; i++)
        frequency[i] = 0;

    for (i = 0; i < in_buffer->len; i++)
        frequency[gch_ubuf(in_buffer, i)]++;
}


void build_frequency_heap(
        struct skew_heap *heap,
        const unsigned long *frequency
) {
    short i;

    for (i = 0; i < ASCII_SIZE; i++)
        if (frequency[i] != 0)
            insert_char_sh(heap, i, frequency[i]);
}


void build_huffman_tree(
        struct huffman_tree *tree,
        struct skew_heap *heap
) {
    struct node_ht *lch, *rch, *parent;
    struct ubuffer temp;

    while (heap->size > 1) {
        lch = pop_sh(heap);
        rch = pop_sh(heap);

        init_ubuf(
            &temp,
            lch->ubuf->buffer,
            lch->ubuf->len
        );
        add_ubuf(&temp, rch->ubuf->buffer, rch->ubuf->len);

        parent = malloc(sizeof(struct node_ht));
        init_empty_node_ht(parent);
        parent->ubuf = malloc(sizeof(struct ubuffer));
        move_ubuf(parent->ubuf, &temp);
        parent->weight = lch->weight + rch->weight;

        insert_ht(tree, lch, rch, parent);
    
        insert_node_sh(heap, parent);
    }
}


void build_code_table(
        struct ubuffer *encrypt_table, 
        struct node_ht *tnode, 
        struct ubuffer *code
) {
    struct ubuffer left_code;
    struct ubuffer right_code;
    unsigned char ch_index;

    if (tnode == NULL)
        return;

    if (tnode->left == NULL && tnode->right == NULL) {
        if (code->len == 0)
            add_ubuf(code, u("0"), 1);
        ch_index = tnode->ubuf->buffer[0];
        copy_ubuf(&encrypt_table[ch_index], code);
        return;
    }

    init_ubuf(&left_code, code->buffer, code->len);
    add_ubuf(&left_code, u("0"), 1);
    build_code_table(encrypt_table, tnode->left, &left_code);
    free_ubuf(&left_code);

    init_ubuf(&right_code, code->buffer, code->len);
    add_ubuf(&right_code, u("1"), 1);
    build_code_table(encrypt_table, tnode->right, &right_code);
    free_ubuf(&right_code);
}


void build_encrypt_table(
        struct ubuffer *encrypt_table,
        struct huffman_tree *tree
) {
    struct ubuffer code;

    init_ubuf(&code, u(""), 0);

    if (tree != NULL)
        build_code_table(encrypt_table, tree->root, &code);

    free_ubuf(&code);
}


void encrypt(
        struct ubuffer *out_buffer,
        const struct ubuffer *in_buffer
) {
    struct ubuffer encrypt_table[ASCII_SIZE];
    struct skew_heap frequency_heap;
    struct huffman_tree code_tree;
    struct ubuffer result, encoded;
    struct bit_writer writer;
    
    unsigned long frequency[ASCII_SIZE];
    unsigned char ch_index;
    unsigned long i;

    if (in_buffer->len == 0) {
        init_ubuf(&result, u(""), 0);
        move_ubuf(out_buffer, &result);
        return;
    }

    build_frequency(frequency, in_buffer);
    
    init_sh(&frequency_heap);
    build_frequency_heap(&frequency_heap, frequency);

    init_ht(&code_tree);
    build_huffman_tree(&code_tree, &frequency_heap);
    code_tree.root = pop_sh(&frequency_heap);
    free_sh(&frequency_heap);

    for (i = 0; i < ASCII_SIZE; i++)
        init_empty_ubuf(&encrypt_table[i]);
    build_encrypt_table(encrypt_table, &code_tree);

    convert_table(&result, encrypt_table);
    free_ht(&code_tree);

    init_ubuf(&encoded, u(""), 0);
    for (i = 0; i < in_buffer->len; i++) {
        ch_index = in_buffer->buffer[i];
        add_ubuf(
            &encoded,
            encrypt_table[ch_index].buffer,
            encrypt_table[ch_index].len
        );
    }

    init_bw(&writer, &encoded);
    free_ubuf(&encoded);
    init_ubuf(&encoded, u(""), 0);
    convert_bits(&encoded, &writer);

    add_long_ubuf(&result, writer.bit_len);
    add_ubuf(&result, u("\n"), 1);
    add_long_ubuf(&result, writer.byte_len);
    add_ubuf(&result, u("\n"), 1);
    add_ubuf(&result, encoded.buffer, encoded.len);
    
    for (i = 0; i < ASCII_SIZE; i++)
        free_ubuf(encrypt_table + i);
    free_ubuf(&encoded);
    free_bw(&writer);

    move_ubuf(out_buffer, &result);
}


void decrypt(
        struct ubuffer *out_buffer,
        const struct ubuffer *in_buffer
) {
    struct ubuffer encrypt_table[ASCII_SIZE];
    struct ubuffer result, bytes;

    unsigned long bits_len, cursor, i;

    cursor = 0;
    deconvert_table(encrypt_table, in_buffer, &cursor);
    
    bits_len = 0;
    for (i = 0; in_buffer->buffer[cursor + i] != '\n'; i++)
        bits_len = bits_len * 10 + (in_buffer->buffer[cursor + i] - '0');
    cursor += i;

    deconvert_bits(&bytes, in_buffer, &cursor);

    
    
    for (i = 0; i < ASCII_SIZE; i++)
        free_ubuf(encrypt_table + i);

    move_ubuf(out_buffer, &result);
}


void archive(const char *out_file, const char *in_file)
{
    struct ubuffer in_file_buffer;
    struct ubuffer encoded_data;

    read_from_file(&in_file_buffer, in_file);
    encrypt(&encoded_data, &in_file_buffer);
    write_to_file(out_file, &encoded_data);

    free_ubuf(&in_file_buffer);
    free_ubuf(&encoded_data);
}
