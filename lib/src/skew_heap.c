/* skew heap realisation */
#include <stdlib.h>

#include "skew_heap.h"
#include "debug.h"

enum gfreq_lib_errs node_sh_init(
		struct node_sh *tnode,
		unsigned char byte,
		unsigned long weight)
{
	int errstat;

	if (tnode == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

    tnode->holder = NULL;
    tnode->left = NULL;
    tnode->right = NULL;
    
	tnode->holder = malloc(sizeof(struct node_ht));
	if (tnode->holder == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_no_memory;
	}

	errstat = node_ht_init(tnode->holder, byte, weight);
	if (errstat != 0) {
		if (tnode->holder != NULL)
			free(tnode->holder);
		return errstat;
	}

	return 0;
}

enum gfreq_lib_errs node_sh_skew_merge(
		struct node_sh **lnode,
		struct node_sh **rnode)
{
	int errstat;

	if (lnode == NULL || rnode == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

    if (*lnode == NULL) {
        *lnode = *rnode;
        *rnode = NULL;
    }
    if (*rnode == NULL)
        return 0;

    if ((*lnode)->holder->weight >= (*rnode)->holder->weight) {
		errstat = node_sh_swap(lnode, rnode);
		if (errstat != 0)
			return errstat;
	}

    errstat = node_sh_skew_merge(&(*lnode)->right, rnode);
	if (errstat != 0)
		return errstat;

	errstat = node_sh_swap(&(*lnode)->left, &(*lnode)->right);
	if (errstat != 0)
		return errstat;

	return 0;
}

enum gfreq_lib_errs node_sh_swap(
		struct node_sh **lnode,
		struct node_sh **rnode)
{
	struct node_sh *tnode;

	if (lnode == NULL || rnode == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

    tnode = *lnode;
    *lnode = *rnode;
    *rnode = tnode;

	return 0;
}

void node_sh_free(struct node_sh *tnode)
{
    if (tnode == NULL)
        return;

    node_sh_free(tnode->left);
    node_sh_free(tnode->right);
	if (tnode != NULL)
    	free(tnode);
}


/* -------------------------------------------------------------------- */


enum gfreq_lib_errs sh_init(struct skew_heap *heap)
{
    if (heap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    heap->peak = NULL;
    heap->size = 0;

	return 0;
}

int sh_is_empty(const struct skew_heap *heap)
{
    if (heap == NULL) {
		GFREQ_DEBUG_MSG;
		return -1;
    }

    return heap->peak == NULL;
}

enum gfreq_lib_errs sh_min(struct skew_heap *heap, struct node_ht **out)
{
    if (heap == NULL || heap->peak == NULL || out == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

	*out = heap->peak->holder;
    return 0;
}

enum gfreq_lib_errs sh_insert(
        struct skew_heap *heap,
        unsigned char byte,
        unsigned long weight
) {
    struct node_sh *tnode;
	int errstat;

    if (heap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }
    
    tnode = malloc(sizeof(struct node_sh));
	if (tnode == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_no_memory;
	}

    errstat = node_sh_init(tnode, byte, weight);
	if (errstat != 0) {
		if (tnode != NULL)
			free(tnode);
		return errstat;
	}

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return 0;
    }

    errstat = node_sh_skew_merge(&heap->peak, &tnode);
	if (errstat != 0)
		return errstat;
    heap->size++;

	return 0;
}

enum gfreq_lib_errs
sh_insert_node(struct skew_heap *heap, struct node_ht *hnode)
{
    struct node_sh *tnode;
	int errstat;

    if (heap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }
   
    tnode = malloc(sizeof(struct node_sh));
	if (tnode == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_no_memory;
	}

    tnode->left = NULL;
	tnode->right = NULL;
    tnode->holder = hnode;

    if (heap->peak == NULL) {
        heap->peak = tnode;
        heap->size++;
        return 0;
    }

    errstat = node_sh_skew_merge(&heap->peak, &tnode);
	if (errstat != 0) {
		if (tnode != NULL)
			free(tnode);
		return errstat;
	}
    heap->size++;

	return 0;
}

enum gfreq_lib_errs sh_pop(struct skew_heap *heap, struct node_ht **result)
{
    struct node_sh *lpeak;
    struct node_sh *rpeak;
	int errstat;
    
    if (heap == NULL || heap->peak == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    lpeak = heap->peak->left;
    rpeak = heap->peak->right;

    *result = heap->peak->holder;
	if (heap->peak != NULL)
    	free(heap->peak);

    heap->peak = lpeak;
    errstat = node_sh_skew_merge(&heap->peak, &rpeak);
	if (errstat != 0)
		return errstat;

    heap->size--;
    return 0;
}

enum gfreq_lib_errs sh_merge(struct skew_heap *lheap, struct skew_heap *rheap)
{
	int errstat;

    if (lheap == NULL || rheap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    lheap->size = lheap->size + rheap->size;
    errstat = node_sh_skew_merge(&lheap->peak, &rheap->peak);
	if (errstat != 0)
		return errstat;

    rheap->peak = NULL;
    rheap->size = 0;

	return 0;
}

enum gfreq_lib_errs sh_free(struct skew_heap *heap)
{
    if (heap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    node_sh_free(heap->peak);

	return 0;
}
