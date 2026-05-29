#include <stdlib.h>
#include <stdio.h>
#include "huffman.h"

void init_empty_node_ht(struct node_ht *holder)
{
    if (holder == NULL)
        return;

    holder->parent = NULL;
    holder->right = NULL;
    holder->left = NULL;
    holder->ubuf = NULL;
    holder->weight = 0;
}

void init_node_ht(
        struct node_ht *holder,
        const unsigned char *ucstr,
        unsigned long len,
        long weight
) {
    if (holder == NULL)
        return;
    
    init_empty_node_ht(holder);

    holder->weight = weight;
    holder->ubuf = malloc(sizeof(struct ubuffer));
    init_ubuf(holder->ubuf, ucstr, len);
}

void print_node_ht(const struct node_ht *tnode, int depth)
{
    int i;

    if (tnode == NULL)
        return;

    print_node_ht(tnode->left, depth + DEPTH_PRINT_FACTOR_HT);
    
    for (i = 0; i < depth; i++)
        putchar(' ');
    printf("('");
    print_ubuf(tnode->ubuf);
    printf("', %ld)\n", tnode->weight);

    print_node_ht(tnode->right, depth + DEPTH_PRINT_FACTOR_HT);
}

void free_node_ht(struct node_ht *holder)
{
    if (holder == NULL)
        return;

    free_node_ht(holder->left);
    free_node_ht(holder->right);

    free_ubuf(holder->ubuf);
    free(holder->ubuf);

    free(holder);
}



void init_ht(struct huffman_tree *tree)
{
    if (tree == NULL)
        return;

    tree->size = 0;
    tree->root = NULL;
}

void insert_ht(
        struct huffman_tree *tree,
        struct node_ht *lch,
        struct node_ht *rch,
        struct node_ht *parent
) {
    if (tree == NULL)
        return;
    
    lch->parent = parent;
    rch->parent = parent;
    parent->left = lch;
    parent->right = rch;

    tree->root = parent;
    tree->size += 2;
}

void print_ht(struct huffman_tree *tree)
{
    if (tree == NULL)
        return;

    print_node_ht(tree->root, 0);
}

void free_ht(struct huffman_tree *tree)
{
    if (tree == NULL)
        return;

    free_node_ht(tree->root);
    init_ht(tree);
}
