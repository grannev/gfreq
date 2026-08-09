#include "crypto.h"


enum gfreq_lib_errs
cm_init(struct crypto_modeule *cm)
{
	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = bio_open(&bio, in_file_name, out_file_name);
	if (errstat != 0)
		return errstat;
	
}

enum gfreq_lib_errs
encrypt(struct crypto_module *cm)
{
	return 0;
}

enum gfreq_lib_errs
decrypt(struct crypto_module *cm)
{
	return 0;
}

enum gfreq_lib_errs
cm_finalize(struct crypto_module *cm)
{
	return 0;
}


