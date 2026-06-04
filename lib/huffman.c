#include <stdlib.h>
#include <stdio.h>

#include "huffman.h"
#include "errs.h"

void node_ht_init_empty(struct node_ht *holder)
{
    if (holder == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_NODE_HT);
    }

    holder->parent = NULL;
    holder->right = NULL;
    holder->left = NULL;
    holder->byte = 0;
    holder->weight = 0;
}

void node_ht_init(
        struct node_ht *holder,
        unsigned char byte,
        unsigned long weight
) {
    if (holder == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_NODE_HT);
    }
    
    node_ht_init_empty(holder);

    holder->weight = weight;
    holder->byte = byte;
}

void node_ht_print(const struct node_ht *tnode, unsigned long depth)
{
    unsigned long i;

    if (tnode == NULL)
        return;

    node_ht_print(tnode->left, depth + DEPTH_PRINT_FACTOR);
    
    for (i = 0; i < depth; i++)
        putchar(' ');
    printf("('%c', %lu)\n", tnode->byte, tnode->weight);

    node_ht_print(tnode->right, depth + DEPTH_PRINT_FACTOR);
}

void node_ht_free(struct node_ht *holder)
{
    if (holder == NULL)
        return;

    node_ht_free(holder->left);
    node_ht_free(holder->right);

    free(holder);
}



void ht_init(struct huffman_tree *tree)
{
    if (tree == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_HUFFMAN_TREE);
    }

    tree->size = 0;
    tree->root = NULL;
}

void ht_insert(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent
) {
    if (tree == NULL) {
        printf(ERR_INSERT_NULL);
        exit(ERR_HUFFMAN_TREE);
    }
    
    lch->parent = parent;
    rch->parent = parent;
    parent->left = lch;
    parent->right = rch;

    tree->root = parent;
    tree->size += 2;
}

void ht_print(const struct huffman_tree *tree)
{
    if (tree == NULL) {
        printf(ERR_PRINT_NULL);
        printf("Object code: %d", ERR_HUFFMAN_TREE);
        return;
    }

    node_ht_print(tree->root, 0);
}

void ht_free(struct huffman_tree *tree)
{
    if (tree == NULL) {
        printf(ERR_FREE_NULL);
        exit(ERR_HUFFMAN_TREE);
    }

    node_ht_free(tree->root);
    ht_init(tree);
}
