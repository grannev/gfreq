#include "../include/errs.h"

/* static const char err_|description|_msg[] = "..." - errors style */
static const char err_null_ptr_msg[] = "trying to reach null pointer";
static const char err_no_file_msg[] = "file not found";
static const char err_open_file_msg[] = "could not open file";
static const char err_eof_file_msg[] = "unexpected end of file";
static const char err_no_memory_msg[] = "memory allocation failed";

static const char err_unknown_msg[] = "unknown error";

const char *gfreq_strerr(enum gfreq_lib_errs error)
{
	switch (error) {
		case gfreq_err_no_file:
			return err_no_file_msg;
		case gfreq_err_open_file:
			return err_open_file_msg;
		case gfreq_err_eof_file:
			return err_eof_file_msg;
		case gfreq_err_null_ptr:
			return err_null_ptr_msg;
		case gfreq_err_no_memory:
			return err_no_memory_msg;
	}

	return err_unknown_msg;
}

void gfreq_printerr(enum gfreq_lib_errs error, FILE *stream)
{
	fputs("ERROR[gfreq-lib]: ", stream);
	fputs(gfreq_strerr(error), stream);
	fputs("\n", stream);
}
