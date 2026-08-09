#include "crypto.h"
#include "debug.h"

enum gfreq_lib_errs
cm_init(struct crypto_module *cm)
{
	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	cm->bio = NULL;
	return 0;
}

enum gfreq_lib_errs
encrypt(struct crypto_module *cm)
{
	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	return 0;
}

enum gfreq_lib_errs
decrypt(struct crypto_module *cm)
{
	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	return 0;
}

enum gfreq_lib_errs
cm_finalize(struct crypto_module *cm)
{
	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	return 0;
}

