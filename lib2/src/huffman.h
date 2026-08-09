#ifndef GFREQ_LIB_HUFFMAN_H
#define GFREQ_LIB_HUFFMAN_H

#include "../include/errs.h"
#include "const.h"

struct node_ht {
    struct node_ht *parent;
    struct node_ht *left;
    struct node_ht *right;
    unsigned char byte;
    unsigned long weight;
};

enum gfreq_lib_errs node_ht_init(
        struct node_ht *holder,
        unsigned char byte,
        unsigned long weight);

void node_ht_free(struct node_ht *holder);

struct huffman_tree {
    struct node_ht *root;
    unsigned long size;
};

enum gfreq_lib_errs ht_init(struct huffman_tree *tree);

enum gfreq_lib_errs ht_insert(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent);

enum gfreq_lib_errs ht_free(struct huffman_tree *tree);

#endif

