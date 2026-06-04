#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "const.h"

/* node_ht struct is likely holder for data in huffman tree */
struct node_ht {
    struct node_ht *parent;
    struct node_ht *left;
    struct node_ht *right;
    unsigned char byte;
    unsigned long weight;
};

/* node_ht_init_empty is initialize empty structure */
void node_ht_init_empty(struct node_ht *holder);

/* node_ht_init is initilize structure with given args */
void node_ht_init(
        struct node_ht *holder,
        unsigned char byte,
        unsigned long weight);

/* node_ht_free is free allocated memory for childrens */
void node_ht_free(struct node_ht *holder);

/* huffman_tree is a struct for realisation of huffman codes algorithm */
struct huffman_tree {
    struct node_ht *root;
    unsigned long size;
};

/* ht_init is initialize tree with nulls */
void ht_init(struct huffman_tree *tree);

/* ht_insert is inserting children and them parent into tree */
void ht_insert(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent);

/* ht_print is used for debug information, prints all nodes of tree */
void ht_print(const struct huffman_tree *tree);

/* ht_free is free allocated memory for nodes of tree */
void ht_free(struct huffman_tree *tree);

#endif

