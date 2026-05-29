#ifndef UBUFFER_H
#define UBUFFER_H

#define CAP_THRESHOLD 256
#define u(cstring) ((const unsigned char*)(cstring))

void copy_ucstr(
        unsigned char *lucstr,
        const unsigned char *rucstr,
        unsigned long len);

void cat_ucstr(
        unsigned char *lucstr,
        const unsigned char *rucstr,
        unsigned long lindex,
        unsigned long rindex);

void rev_ucstr(
        unsigned char *ucstr,
        unsigned long left,
        unsigned long right);

struct ubuffer {
    unsigned char *buffer;
    unsigned long len;
    unsigned long cap;
};

void init_empty_ubuf(struct ubuffer *ubuf);

void init_ubuf(
        struct ubuffer *ubuf,
        const unsigned char *ucstr,
        unsigned long len);

void add_ubuf(
        struct ubuffer *ubuf,
        const unsigned char *ucstr,
        unsigned long len);

void add_long_ubuf(struct ubuffer *ubuf, long value);

void print_ubuf(struct ubuffer *ubuf);

unsigned char gch_ubuf(const struct ubuffer *ubuf, unsigned long index);

void copy_ubuf(struct ubuffer *lubuf, struct ubuffer *rubuf);

void move_cubuf(unsigned char **lcstr, unsigned char **rcstr);

void move_ubuf(struct ubuffer *lubuf, struct ubuffer *rubuf);

void free_ubuf(struct ubuffer *ubuf);

#endif

