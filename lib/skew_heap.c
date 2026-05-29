/* skew heap realisation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skew_heap.h"

void init_empty_node_sh(struct node_sh *tnode)
{
    tnode->holder = NULL;
    tnode->left = NULL;
    tnode->right = NULL;
}

void init_node_sh(
        struct node_sh *tnode,
        const unsigned char *cstr,
        unsigned long len, 
        long weight
) {
    tnode->left = NULL;
    tnode->right = NULL;
    tnode->holder = malloc(sizeof(struct node_ht));
    init_node_ht(tnode->holder, cstr, len, weight);
}

void skew_merge_node_sh(struct node_sh **lnode, struct node_sh **rnode)
{
    if (*lnode == NULL) {
        *lnode = *rnode;
        *rnode = NULL;
    }
    if (*rnode == NULL) {
        return;
    }

    if ((*lnode)->holder->weight >= (*rnode)->holder->weight)
        swap_node_sh(lnode, rnode);
    skew_merge_node_sh(&(*lnode)->right, rnode);

    swap_node_sh(&(*lnode)->left, &(*lnode)->right);
}

void swap_node_sh(struct node_sh **lnode, struct node_sh **rnode)
{
    struct node_sh *tnode;
    tnode = *lnode;
    *lnode = *rnode;
    *rnode = tnode;
}

void print_node_sh(const struct node_sh *tnode, long depth)
{
    int i;

    if (tnode == NULL)
        return;

    print_node_sh(tnode->left, depth + DEPTH_PRINT_FACTOR_SH);
    
    for (i = 0; i < depth; i++)
        putchar(' ');
    printf("('");
    print_ubuf(tnode->holder->ubuf);
    printf("', %ld)\n", tnode->holder->weight);

    print_node_sh(tnode->right, depth + DEPTH_PRINT_FACTOR_SH);
}

void clone_node_sh(struct node_sh *lnode, const struct node_sh *rnode)
{
    if (rnode == NULL) {
        lnode = NULL;
        return;
    }

    lnode->holder = malloc(sizeof(struct node_ht));
    init_node_ht(
        lnode->holder,
        rnode->holder->ubuf->buffer,
        rnode->holder->ubuf->len,
        rnode->holder->weight
    );

    if (rnode->left != NULL) {
        lnode->left = malloc(sizeof(struct node_sh));
        clone_node_sh(lnode->left, rnode->left);
    }

    if (rnode->right != NULL) {
        lnode->right = malloc(sizeof(struct node_sh));
        clone_node_sh(lnode->right, rnode->right);
    }
}

void free_node_sh(struct node_sh *tnode)
{
    if (tnode == NULL)
        return;

    free_node_sh(tnode->left);
    free_node_sh(tnode->right);
    free(tnode);
}


/* -------------------------------------------------------------------- */


void init_sh(struct skew_heap *heap)
{
    if (heap == NULL) {
        perror("Trying init NULL");
        return;
    }
    heap->peak = NULL;
    heap->size = 0;
}

int is_empty_sh(struct skew_heap *heap)
{
    if (heap == NULL)
        return 0;
    return heap->peak == NULL;
}

struct node_ht *min_sh(struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        perror("Trying get min from NULL");
        return NULL;
    }
    return heap->peak->holder;
}

long min_weight_sh(struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        perror("Trying get min from NULL");
        return 0;
    }
    return heap->peak->holder->weight;
}

unsigned char *min_cstr_sh(struct skew_heap *heap)
{
    if (heap == NULL || heap->peak == NULL) {
        perror("Trying get min from NULL");
        return NULL;
    }
    return heap->peak->holder->ubuf->buffer;
}

void insert_sh(
        struct skew_heap *heap,
        const unsigned char *ucstr,
        unsigned long len,
        long weight
) {
    struct node_sh *tnode;

    if (heap == NULL) {
        perror("Trying insert to NULL");
        return;
    }
    
    tnode = malloc(sizeof(struct node_sh));
    init_node_sh(tnode, ucstr, len, weight);

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return;
    }

    skew_merge_node_sh(&heap->peak, &tnode);
    heap->size++;
}

void insert_char_sh(struct skew_heap *heap, char ch, long weight)
{
    struct node_sh *tnode;
    char cstr[1];

    if (heap == NULL) {
        perror("Trying insert to NULL");
        return;
    }
    
    cstr[0] = ch;
    tnode = malloc(sizeof(struct node_sh));
    init_node_sh(tnode, (const unsigned char *)cstr, 1, weight);

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return;
    }

    skew_merge_node_sh(&heap->peak, &tnode);
    heap->size++;
}

void insert_node_sh(struct skew_heap *heap, struct node_ht *hnode)
{
    struct node_sh *tnode;

    if (heap == NULL) {
        perror("Trying insert to NULL");
        return;
    }
   
    tnode = malloc(sizeof(struct node_sh));
    init_empty_node_sh(tnode);
    tnode->holder = hnode;

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return;
    }

    skew_merge_node_sh(&heap->peak, &tnode);
    heap->size++;
}

struct node_ht *pop_sh(struct skew_heap *heap)
{
    struct node_sh *lpeak;
    struct node_sh *rpeak;
    struct node_ht *thold;
    
    if (heap == NULL || heap->peak == NULL) {
        perror("Nothing to pop from skew heap");
        return NULL;
    }

    lpeak = heap->peak->left;
    rpeak = heap->peak->right;
    thold = heap->peak->holder;

    free(heap->peak);

    heap->peak = lpeak;
    skew_merge_node_sh(&heap->peak, &rpeak);

    heap->size--;
    return thold;
}

void print_sh(struct skew_heap *heap)
{
    if (heap == NULL)
        return;

    print_node_sh(heap->peak, 0);
}

void merge_sh(struct skew_heap *lheap, struct skew_heap *rheap)
{
    if (lheap == NULL || rheap == NULL) {
        perror("Trying merge from NULL or/and to NULL");
        return;
    }

    lheap->size = lheap->size + rheap->size;
    skew_merge_node_sh(&lheap->peak, &rheap->peak);

    rheap->peak = NULL;
    rheap->size = 0;
}

void free_sh(struct skew_heap *heap)
{
    if (heap == NULL) {
        perror("Trying free NULL");
        return;
    }

    free_node_sh(heap->peak);
}
