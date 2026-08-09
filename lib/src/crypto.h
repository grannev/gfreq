#ifndef GFREQ_CRYPTO_H
#define GFREQ_CRYPTO_H

#include "../include/errs.h"
#include "byte_io.h"

struct crypto_module {
	struct byte_io *bio;
};


enum gfreq_lib_errs
cm_init(struct crypto_module *cm);

enum gfreq_lib_errs
encrypt(struct crypto_module *cm);

enum gfreq_lib_errs
decrypt(struct crypto_module *cm);

enum gfreq_lib_errs
cm_finalize(struct crypto_module *cm);

#endif
