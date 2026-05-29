#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digmath.h"
#include "ubuffer.h"

void copy_ucstr(
        unsigned char *lucstr,
        const unsigned char *rucstr,
        unsigned long len
) {
    unsigned long i;
    for (i = 0; i < len; i++)
        lucstr[i] = rucstr[i];
}

void cat_ucstr(
        unsigned char *lucstr,
        const unsigned char *rucstr,
        unsigned long lindex,
        unsigned long rindex
) {
    unsigned long i, j;
    for (i = lindex, j = 0; j < rindex; i++, j++)
        lucstr[i] = rucstr[j];
}

void rev_ucstr(
        unsigned char *ucstr,
        unsigned long left,
        unsigned long right
) {
    while (left < right) {
        swap_uchar(ucstr + left, ucstr + right);
        left++;
        right--;
    }
}

int next_cap_buff(int new_len, int old_cap)
{
    int new_cap;
    int double_cap;

    new_cap = old_cap;
    double_cap = new_cap + new_cap;

    if (new_len > double_cap)
        return new_len;

    if (old_cap < CAP_THRESHOLD)
        return double_cap;

    while (new_cap < new_len)
        new_cap += (new_cap + 3 * CAP_THRESHOLD) >> 2;
    
    if (new_cap <= 0)
        return new_len;

    return new_cap;
}


void init_empty_ubuf(struct ubuffer *ubuf)
{
    if (ubuf == NULL)
        return;

    ubuf->buffer = NULL;
    ubuf->cap = 0;
    ubuf->len = 0;
}

void init_ubuf(
        struct ubuffer *ubuf,
        const unsigned char *ucstr,
        unsigned long len
) {
    if (ubuf == NULL || ucstr == NULL)
        return;

    init_empty_ubuf(ubuf);

    if (len == 0) {
        ubuf->buffer = malloc(8);
        ubuf->len = len;
        ubuf->cap = 8;
        return;
    }

    ubuf->cap = next_cap_buff(len, ubuf->cap);
    ubuf->len = len;
    
    ubuf->buffer = malloc(ubuf->cap);
    copy_ucstr(ubuf->buffer, ucstr, len);
}

void add_ubuf(
        struct ubuffer *ubuf,
        const unsigned char *ucstr,
        unsigned long len
) {
    unsigned char *new_buffer;
    int new_len;

    if (ubuf == NULL || ucstr == NULL)
        return;

    new_len = ubuf->len + len;
    ubuf->cap = next_cap_buff(new_len, ubuf->cap);
    new_buffer = realloc(ubuf->buffer, ubuf->cap);

    ubuf->buffer = new_buffer;
    cat_ucstr(
    ubuf->buffer,
    ucstr,
    ubuf->len,
    len
    );
    ubuf->len = new_len;
}

void add_long_ubuf(struct ubuffer *ubuf, long value)
{
    unsigned long len;
    unsigned char ch[1];

    len = 0;
    while (value > 0) {
        ch[0] = (unsigned char) (value % 10 + '0');
        add_ubuf(ubuf, ch, 1);
        value /= 10;
        len++;
    }

    rev_ucstr(
        ubuf->buffer,
        ubuf->len - len,
        ubuf->len - 1);
}

void print_ubuf(struct ubuffer *ubuf)
{
    unsigned long i;

    for (i = 0; i < ubuf->len; i++)
        putchar(ubuf->buffer[i]);
    putchar('\n');
}

unsigned char gch_ubuf(const struct ubuffer *ubuf, unsigned long index)
{
    return ubuf->buffer[index];
}

void copy_ubuf(struct ubuffer *lubuf, struct ubuffer *rubuf)
{
    if (lubuf == NULL || rubuf == NULL)
        return;

    init_ubuf(lubuf, rubuf->buffer, rubuf->len);
}

void move_cubuf(unsigned char **lcstr, unsigned char **rcstr)
{
    *lcstr = *rcstr;
    *rcstr = NULL;
}

void move_ubuf(struct ubuffer *lubuf, struct ubuffer *rubuf)
{
    if (lubuf == NULL || rubuf == NULL)
        return;

    lubuf->buffer = rubuf->buffer;
    lubuf->cap = rubuf->cap;
    lubuf->len = rubuf->len;
    init_empty_ubuf(rubuf);
}

void free_ubuf(struct ubuffer *ubuf)
{
    if (ubuf == NULL)
        return;
    if (ubuf->buffer != NULL)
        free(ubuf->buffer);
    init_empty_ubuf(ubuf);
}
