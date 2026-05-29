#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "ubuffer.h"

#define DEPTH_PRINT_FACTOR_HT 12

struct node_ht {
    struct node_ht *parent;
    struct node_ht *left;
    struct node_ht *right;
    struct ubuffer *ubuf;
    long weight;
};

void init_empty_node_ht(struct node_ht *holder);

void init_node_ht(
        struct node_ht *holder,
        const unsigned char *ucstr,
        unsigned long len,
        long weight);

void print_node_ht(const struct node_ht *tnode, int depth);

void free_node_ht(struct node_ht *holder);


struct huffman_tree {
    struct node_ht *root;
    int size;
};

void init_ht(struct huffman_tree *tree);

void insert_ht(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent);

void print_ht(struct huffman_tree *tree);

void free_ht(struct huffman_tree *tree);

#endif

