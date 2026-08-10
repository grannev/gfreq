#ifndef GFREQ_CRYPTO_H
#define GFREQ_CRYPTO_H

#include "../include/errs.h"
#include "skew_heap.h"
#include "huffman.h"
#include "byte_io.h"
#include "const.h"

struct crypto_module {
	struct byte_io *bio;
	struct skew_heap *heap;
	struct huffman_tree *tree;
	unsigned long cryptable[byte_cnt];
	unsigned long frequency[byte_cnt];
	unsigned long codes_len[byte_cnt];
	unsigned long bits, bytes;
};


enum gfreq_lib_errs
cm_init(
		struct crypto_module *cm,
		const char *in_file_name,
		const char *out_file_name);

enum gfreq_lib_errs
encrypt(struct crypto_module *cm);

enum gfreq_lib_errs
decrypt(struct crypto_module *cm);

enum gfreq_lib_errs
cm_finalize(struct crypto_module *cm);

#endif
