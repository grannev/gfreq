/* skew heap realisation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skew_heap.h"
#include "errs.h"

void node_sh_init_empty(struct node_sh *tnode)
{
    if (tnode == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_NODE_SH);
    }

    tnode->holder = NULL;
    tnode->left = NULL;
    tnode->right = NULL;
}

void node_sh_init(
        struct node_sh *tnode,
        unsigned char byte,
        unsigned long weight
) {
    if (tnode == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_NODE_SH);
    }

    node_sh_init_empty(tnode);
    tnode->holder = malloc(sizeof(struct node_ht));
    node_ht_init(tnode->holder, byte, weight);
}

void node_sh_skew_merge(struct node_sh **lnode, struct node_sh **rnode)
{
    if (*lnode == NULL) {
        *lnode = *rnode;
        *rnode = NULL;
    }
    if (*rnode == NULL) {
        return;
    }

    if ((*lnode)->holder->weight >= (*rnode)->holder->weight)
        node_sh_swap(lnode, rnode);
    node_sh_skew_merge(&(*lnode)->right, rnode);

    node_sh_swap(&(*lnode)->left, &(*lnode)->right);
}

void node_sh_swap(struct node_sh **lnode, struct node_sh **rnode)
{
    struct node_sh *tnode;
    tnode = *lnode;
    *lnode = *rnode;
    *rnode = tnode;
}

void node_sh_print(const struct node_sh *tnode, unsigned long depth)
{
    int i;

    if (tnode == NULL)
        return;

    node_sh_print(tnode->left, depth + DEPTH_PRINT_FACTOR);
    
    for (i = 0; i < depth; i++)
        putchar(' ');
    printf("('%c', %lu)\n", tnode->holder->byte, tnode->holder->weight);

    node_sh_print(tnode->right, depth + DEPTH_PRINT_FACTOR);
}

void node_sh_free(struct node_sh *tnode)
{
    if (tnode == NULL)
        return;

    node_sh_free(tnode->left);
    node_sh_free(tnode->right);
    free(tnode);
}


/* -------------------------------------------------------------------- */


void sh_init(struct skew_heap *heap)
{
    if (heap == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_SKEW_HEAP);
    }
    heap->peak = NULL;
    heap->size = 0;
}

int sh_is_empty(const struct skew_heap *heap)
{
    if (heap == NULL) {
        printf(ERR_INFO_NULL);
        exit(ERR_SKEW_HEAP);
    }
    return heap->peak == NULL;
}

struct node_ht *sh_min(struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        printf(ERR_INFO_NULL);
        exit(ERR_SKEW_HEAP);
    }
    return heap->peak->holder;
}

long sh_min_weight(const struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        printf(ERR_INFO_NULL);
        exit(ERR_SKEW_HEAP);
    }
    return heap->peak->holder->weight;
}

unsigned char sh_min_byte(const struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        printf(ERR_INFO_NULL);
        exit(ERR_SKEW_HEAP);
    }
    return heap->peak->holder->byte;
}

void sh_insert(
        struct skew_heap *heap,
        unsigned char byte,
        unsigned long weight
) {
    struct node_sh *tnode;

    if (heap == NULL) {
        printf(ERR_INSERT_NULL);
        exit(ERR_SKEW_HEAP);
    }
    
    tnode = malloc(sizeof(struct node_sh));
    node_sh_init(tnode, byte, weight);

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return;
    }

    node_sh_skew_merge(&heap->peak, &tnode);
    heap->size++;
}

void sh_insert_node(struct skew_heap *heap, struct node_ht *hnode)
{
    struct node_sh *tnode;

    if (heap == NULL) {
        printf(ERR_INSERT_NULL);
        exit(ERR_SKEW_HEAP);
    }
   
    tnode = malloc(sizeof(struct node_sh));
    node_sh_init_empty(tnode);
    tnode->holder = hnode;

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return;
    }

    node_sh_skew_merge(&heap->peak, &tnode);
    heap->size++;
}

void sh_pop(struct skew_heap *heap, struct node_ht **result)
{
    struct node_sh *lpeak;
    struct node_sh *rpeak;
    
    if (heap == NULL || heap->peak == NULL) {
        printf(ERR_POP_NULL);
        exit(ERR_SKEW_HEAP);
    }

    lpeak = heap->peak->left;
    rpeak = heap->peak->right;

    *result = heap->peak->holder;
    free(heap->peak);

    heap->peak = lpeak;
    node_sh_skew_merge(&heap->peak, &rpeak);

    heap->size--;
    return;
}

void sh_print(const struct skew_heap *heap)
{
    if (heap == NULL) {
        printf(ERR_PRINT_NULL);
        printf("Object code: %d", ERR_SKEW_HEAP);
        return;
    }

    node_sh_print(heap->peak, 0);
}

void sh_merge(struct skew_heap *lheap, struct skew_heap *rheap)
{
    if (lheap == NULL || rheap == NULL) {
        printf(ERR_MERGE_NULL);
        exit(ERR_SKEW_HEAP);
    }

    lheap->size = lheap->size + rheap->size;
    node_sh_skew_merge(&lheap->peak, &rheap->peak);

    rheap->peak = NULL;
    rheap->size = 0;
}

void sh_free(struct skew_heap *heap)
{
    if (heap == NULL) {
        printf(ERR_FREE_NULL);
        exit(ERR_SKEW_HEAP);
    }

    node_sh_free(heap->peak);
}
