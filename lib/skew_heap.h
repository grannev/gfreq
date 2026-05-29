/* skew heap header */
#ifndef SKEW_HEAP_HEADER
#define SKEW_HEAP_HEADER

#include "huffman.h"

#define DEPTH_PRINT_FACTOR_SH 6

struct node_sh {
    struct node_sh *left;
    struct node_sh *right;
    struct node_ht *holder;
};

void init_empty_node_sh(struct node_sh *tnode);

void init_node_sh(
        struct node_sh *tnode,
        const unsigned char *cstr,
        unsigned long len,
        long weight);

void skew_merge_node_sh(struct node_sh **lnode, struct node_sh **rnode);

void swap_node_sh(struct node_sh **lnode, struct node_sh **rnode);

void print_node_sh(const struct node_sh *tnode, long depth);

void clone_node_sh(struct node_sh *lnode, const struct node_sh *rnode);

void free_node_sh(struct node_sh *tnode);


struct skew_heap {
    long size;
    struct node_sh *peak;
};

void init_sh(struct skew_heap *heap);

int is_empty_sh(struct skew_heap *heap);

struct node_ht *min_sh(struct skew_heap *heap);

long min_weight_sh(struct skew_heap *heap);

unsigned char *min_cstr_sh(struct skew_heap *heap);

void insert_sh(
        struct skew_heap *heap,
        const unsigned char *ucstr,
        unsigned long len,
        long weight);

void insert_char_sh(struct skew_heap *heap, char ch, long weight);

void insert_node_sh(struct skew_heap *heap, struct node_ht *tnode);

struct node_ht *pop_sh(struct skew_heap *heap);

void print_sh(struct skew_heap *heap);

void merge_sh(struct skew_heap *lheap, struct skew_heap *rheap);

void free_sh(struct skew_heap *heap);


#endif

