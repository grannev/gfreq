#include <stdlib.h>

#include "huffman.h"
#include "debug.h"

enum gfreq_lib_errs node_ht_init(
        struct node_ht *holder,
        unsigned char byte,
        unsigned long weight
) {
	if (holder == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

    holder->parent = NULL;
    holder->right = NULL;
    holder->left = NULL;

    holder->weight = weight;
    holder->byte = byte;

	return 0;
}

void node_ht_free(struct node_ht *holder)
{
	if (holder == NULL)
		return;

    node_ht_free(holder->left);
    node_ht_free(holder->right);

	if (holder != NULL)
    	free(holder);
}


enum gfreq_lib_errs ht_init(struct huffman_tree *tree)
{
    if (tree == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    tree->size = 0;
    tree->root = NULL;

	return 0;
}

enum gfreq_lib_errs ht_insert(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent
) {
    if (tree == NULL || lch == NULL || rch == NULL || parent == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }
    
    lch->parent = parent;
    rch->parent = parent;
    parent->left = lch;
    parent->right = rch;

    tree->root = parent;
    tree->size += 2;

	return 0;
}

enum gfreq_lib_errs ht_free(struct huffman_tree *tree)
{
    int errstat;

	if (tree == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
    }

    node_ht_free(tree->root);
	errstat = ht_init(tree);
	if (errstat != 0)
		return errstat;

	return 0;
}

