#include "../include/gfreq.h"
#include "crypto.h"
#include "debug.h"

enum gfreq_lib_errs
gfreq_pack_file(const char *in_file_name, const char *out_file_name)
{
	struct crypto_module cm;
	enum gfreq_lib_errs errstat;

	if (in_file_name == NULL || out_file_name == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = cm_init(&cm, in_file_name, out_file_name);
	if (errstat != 0)
		return errstat;

	errstat = encrypt(&cm);
	if (errstat != 0)
		return errstat;

	errstat = cm_finalize(&cm);
	if (errstat != 0)
		return errstat;

	return 0;
}

enum gfreq_lib_errs
gfreq_unpack_file(const char *in_file_name, const char *out_file_name)
{
	struct crypto_module crypto;
	enum gfreq_lib_errs errstat;

	if (in_file_name == NULL || out_file_name == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = cm_init(&crypto, in_file_name, out_file_name);
	if (errstat != 0)
		return errstat;

	errstat = decrypt(&crypto);
	if (errstat != 0)
		return errstat;

	errstat = cm_finalize(&crypto);
	if (errstat != 0)
		return errstat;

	return 0;
}
