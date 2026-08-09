#ifndef GFREQ_BYTE_INPUT_OUTPUT_H
#define GFREQ_BYTE_INPUT_OUTPUT_H

#include <stdio.h>
#include "../include/errs.h"

/* byte_i(nput)o(utput) struct for work with bytes in file */
struct byte_io {
    FILE *in;
    FILE *out;
    unsigned long in_len;
};

enum gfreq_lib_errs
bio_open(struct byte_io *bio,
		const char *in_file_name,
		const char *out_file_name);

enum gfreq_lib_errs
bio_read_ulong(struct byte_io *bio, unsigned long *value);

enum gfreq_lib_errs
bio_read_uch(struct byte_io *bio, unsigned char *value);

enum gfreq_lib_errs
bio_write_ulong(struct byte_io *bio, unsigned long value);

enum gfreq_lib_errs
bio_write_uch(struct byte_io *bio, unsigned char value);

enum gfreq_lib_errs
bio_close(struct byte_io *bio);

#endif
