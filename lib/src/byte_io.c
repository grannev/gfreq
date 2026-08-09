#include "byte_io.h"
#include "debug.h"

enum gfreq_lib_errs
bio_open(struct byte_io *bio,
		const char *in_file_name,
		const char *out_file_name)
{
	if (bio == NULL || in_file_name == NULL || out_file_name == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	bio->in = fopen(in_file_name, "rb");
	bio->out = fopen(out_file_name, "wb");

	if (bio->in == NULL || bio->out == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_open_file;
	}

	return 0;
}

enum gfreq_lib_errs
bio_read_ulong(struct byte_io *bio, unsigned long *value)
{
	if (bio == NULL || bio->in == NULL || value == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (fscanf(bio->in, "%lu", value) != 1) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_eof_file;
	}

	return 0;
}

enum gfreq_lib_errs
bio_read_uch(struct byte_io *bio, unsigned char *value)
{
	int current_byte;

	if (bio == NULL || bio->in == NULL || value == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	current_byte = fgetc(bio->in);
	if (current_byte == EOF) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_eof_file;
	}

	*value = (unsigned char) current_byte;
	return 0;
}

enum gfreq_lib_errs
bio_write_ulong(struct byte_io *bio, unsigned long value)
{
	if (bio == NULL || bio->out == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (fprintf(bio->out, "%lu", value) < 0) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_eof_file;
	}

	return 0;
}

enum gfreq_lib_errs
bio_write_uch(struct byte_io *bio, unsigned char value)
{
	if (bio == NULL || bio->out == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (fputc(value, bio->out) == EOF) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_eof_file;
	}

	return 0;
}

enum gfreq_lib_errs
bio_close(struct byte_io *bio)
{
	if (bio == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (fclose(bio->in) == EOF || fclose(bio->out) == EOF) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_eof_file;
	}

	return 0;
}
