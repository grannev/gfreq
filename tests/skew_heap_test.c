#include "skew_heap.h"
#include <stdio.h>


int main()
{
    int i;
    char *ubuffer = "Lorom ipsum lorso irum dolor sit amel";
    long frequency[256] = {0};

    skew_heap heap;
    init_sh(&heap);

    for (i = 0; ubuffer[i] != '\0'; i++)
        frequency[ubuffer[i]]++;

    for (i = 0; i < 256; i++)
        if (frequency[i] != 0)
            insert_sh(&heap, (char) i, frequency[i]);

    /* print_node_sh(heap.peak, 0); */

    while (!is_empty_sh(&heap)) {
        printf("(%c, %ld)", min_symbol_sh(&heap), min_weight_sh(&heap));
        pop_sh(&heap);
    }
    putchar('\n');

    free_sh(&heap);
    return 0;
}

