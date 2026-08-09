#ifndef GFREQ_LIB_ERRS_H
#define GFREQ_LIB_ERRS_H

#include <stdio.h>

enum gfreq_lib_errs {
	gfreq_err_no_file = 5,
	gfreq_err_open_file = 6,
	gfreq_err_eof_file = 7,

	gfreq_err_null_ptr = 15,
	gfreq_err_no_memory = 16
};

/* Translate integer code of error to description string */
const char *gfreq_strerr(enum gfreq_lib_errs error);

/* Prints description of error by his code */
void gfreq_printerr(enum gfreq_lib_errs error, FILE *stream);

#endif
