/* skew heap header */
#ifndef SKEW_HEAP_HEADER
#define SKEW_HEAP_HEADER

#include "const.h"
#include "huffman.h"

/* node_sh is struct of node for skew_heap struct */
struct node_sh {
    struct node_sh *left;
    struct node_sh *right;
    struct node_ht *holder;
};

/* node_sh_init_empty is initializing node with null values */
void node_sh_init_empty(struct node_sh *tnode);

/* node_sh_init is initializing struct with given args */
void node_sh_init(
        struct node_sh *tnode,
        unsigned char byte,
        unsigned long weight);

/* node_sh_skew_merge is doing skew_merge algorithm with two of nodes */
void node_sh_skew_merge(struct node_sh **lnode, struct node_sh **rnode);

/* node_sh_swap swapes given nodes by pointer to pointer to args */
void node_sh_swap(struct node_sh **lnode, struct node_sh **rnode);

/* node_sh_print is for debug, prints this node and his children */
void node_sh_print(const struct node_sh *tnode, unsigned long depth);

/* node_sh_free is free allocated data for children */
void node_sh_free(struct node_sh *tnode);


/* skew_heap is needed to get two mins objects 
 * for huffman algorithm realization */
struct skew_heap {
    unsigned long size;
    struct node_sh *peak;
};

/* sh_init is initialize heap with null values */
void sh_init(struct skew_heap *heap);

/* sh_is_empty return boolean value of heap emptiness */
int sh_is_empty(const struct skew_heap *heap);

/* sh_min return holder pointer */
struct node_ht *sh_min(struct skew_heap *heap);

/* sh_min_weight return weight of holder */
long sh_min_weight(const struct skew_heap *heap);

/* sh_min_weight return byte of holder */
unsigned char sh_min_byte(const struct skew_heap *heap);

/* sh_insert is inserting values to heap given by args */
void sh_insert(
        struct skew_heap *heap,
        unsigned char byte,
        unsigned long weight);

/* sh_insert_node is insert holder to heap */
void sh_insert_node(struct skew_heap *heap, struct node_ht *tnode);

/* sh_pop is removing peak of heap and moves his holder to result */
void sh_pop(struct skew_heap *heap, struct node_ht **result);

/* sh_print is need for debug, it prints all nodes of heap */
void sh_print(const struct skew_heap *heap);

/* sh_merge is wrapper for node_sh_skew_merge for peaks of heaps */
void sh_merge(struct skew_heap *lheap, struct skew_heap *rheap);

/* sh_free is free allocated memory for nodes of heap */
void sh_free(struct skew_heap *heap);


#endif

