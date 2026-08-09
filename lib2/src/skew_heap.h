/* skew heap header */
#ifndef GFREQ_LIB_SKEW_HEAP_H
#define GFREQ_LIB_SKEW_HEAP_H

#include "../include/errs.h"
#include "huffman.h"
#include "const.h"

struct node_sh {
    struct node_sh *left;
    struct node_sh *right;
    struct node_ht *holder;
};

enum gfreq_lib_errs node_sh_init(
        struct node_sh *tnode,
        unsigned char byte,
        unsigned long weight);

enum gfreq_lib_errs node_sh_skew_merge(
		struct node_sh **lnode,
		struct node_sh **rnode);

enum gfreq_lib_errs node_sh_swap(
		struct node_sh **lnode,
		struct node_sh **rnode);

void node_sh_free(struct node_sh *tnode);


struct skew_heap {
    unsigned long size;
    struct node_sh *peak;
};

enum gfreq_lib_errs sh_init(struct skew_heap *heap);

int sh_is_empty(const struct skew_heap *heap);

enum gfreq_lib_errs sh_min(struct skew_heap *heap, struct node_ht *out);

enum gfreq_lib_errs sh_insert(
        struct skew_heap *heap,
        unsigned char byte,
        unsigned long weight);

enum gfreq_lib_errs sh_insert_node(struct skew_heap *heap, struct node_ht *tnode);

enum gfreq_lib_errs sh_pop(struct skew_heap *heap, struct node_ht **result);

enum gfreq_lib_errs sh_merge(struct skew_heap *lheap, struct skew_heap *rheap);

enum gfreq_lib_errs sh_free(struct skew_heap *heap);

#endif

