#ifndef BYTE_WRITER_H
#define BYTE_WRITER_H

#include <stdio.h>

/* byte_r(ead)w(rite) struct for work with bytes in file */
struct byte_rw {
    FILE *in;
    FILE *out;
    unsigned long in_len;
    unsigned long out_len;
    int in_eof;
};

/* brw_init is open files by names for read and write */
void brw_init(
        struct byte_rw *brw,
        const char *in_name,
        const char *out_name);

/* brw_next reading next byte from FILE *in */
unsigned char brw_next(struct byte_rw *brw);

/* brw_next_ulong reading next unsigned long digit from FILE *in */
unsigned long brw_next_ulong(struct byte_rw *brw);

/* brw_put_uch puts a byte in FILE *out */
void brw_put_uch(struct byte_rw *brw, unsigned char uch);

/* brw_put_uch puts a unsigned long digit in FILE *out */
void brw_put_ulong(struct byte_rw *brw, unsigned long value);

/* brw_close fclose both FILE *in and FILE *out */
void brw_close(struct byte_rw *brw);

#endif

