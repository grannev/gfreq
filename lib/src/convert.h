#ifndef GFREQ_CONVERT_H
#define GFREQ_CONVERT_H

#include "byte_io.h"
#include "huffman.h"

/* Writes the frequency table to the output stream. */
enum gfreq_lib_errs convert_table(
		struct byte_io *bio,
		const unsigned long *frequency);

/* Reads the frequency table from the input stream. */
enum gfreq_lib_errs deconvert_table(
		struct byte_io *bio,
		unsigned long *frequency);

/* Encodes the input stream and writes packed bits to the output stream. */
enum gfreq_lib_errs convert_bits(
		struct byte_io *bio,
		const unsigned long *encrypt_table,
		const unsigned long *codes_len);

/* Decodes packed bits and writes the original bytes to the output stream. */
enum gfreq_lib_errs deconvert_bits(
		struct byte_io *bio,
		const struct huffman_tree *tree,
		unsigned long bits_len,
		unsigned long bytes_len);

#endif
