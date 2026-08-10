#include <stdio.h>

#include "convert.h"
#include "const.h"
#include "debug.h"

enum gfreq_lib_errs convert_table(
		struct byte_io *bio,
		const unsigned long *frequency)
{
	enum gfreq_lib_errs errstat;
	unsigned long i;
	unsigned long table_size;

	if (bio == NULL || frequency == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	table_size = 0;
	for (i = 0; i < byte_cnt; i++) {
		if (frequency[i] != 0)
			table_size++;
	}

	errstat = bio_write_ulong(bio, table_size);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_uch(bio, '\n');
	if (errstat != 0)
		return errstat;

	for (i = 0; i < byte_cnt; i++) {
		if (frequency[i] == 0)
			continue;

#ifdef BEAUTIFUL_TABLE
		if (i != '\n') {
			errstat = bio_write_uch(bio, '\'');
			if (errstat != 0)
				return errstat;
#endif
			errstat = bio_write_uch(bio, (unsigned char) i);
			if (errstat != 0)
				return errstat;
#ifdef BEAUTIFUL_TABLE
			errstat = bio_write_uch(bio, '\'');
			if (errstat != 0)
				return errstat;
		} else {
			errstat = bio_write_uch(bio, '\\');
			if (errstat != 0)
				return errstat;

			errstat = bio_write_uch(bio, 'n');
			if (errstat != 0)
				return errstat;
		}

		errstat = bio_write_uch(bio, ' ');
		if (errstat != 0)
			return errstat;
#endif

		errstat = bio_write_uch(bio, ' ');
		if (errstat != 0)
			return errstat;

		errstat = bio_write_ulong(bio, frequency[i]);
		if (errstat != 0)
			return errstat;

		errstat = bio_write_uch(bio, '\n');
		if (errstat != 0)
			return errstat;
	}

#ifdef BEAUTIFUL_TABLE
	errstat = bio_write_uch(bio, '\n');
	if (errstat != 0)
		return errstat;
#endif

	return 0;
}

enum gfreq_lib_errs deconvert_table(
		struct byte_io *bio,
		unsigned long *frequency)
{
	enum gfreq_lib_errs errstat;
	unsigned long table_size;
	unsigned long i;
	unsigned char byte;
	unsigned char separator;

	if (bio == NULL || frequency == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	for (i = 0; i < byte_cnt; i++)
		frequency[i] = 0;

	errstat = bio_read_ulong(bio, &table_size);
	if (errstat != 0)
		return errstat;

	errstat = bio_read_uch(bio, &separator);
	if (errstat != 0)
		return errstat;

	for (i = 0; i < table_size; i++) {
		errstat = bio_read_uch(bio, &byte);
		if (errstat != 0)
			return errstat;

		errstat = bio_read_uch(bio, &separator);
		if (errstat != 0)
			return errstat;

		errstat = bio_read_ulong(bio, &frequency[byte]);
		if (errstat != 0)
			return errstat;

		errstat = bio_read_uch(bio, &separator);
		if (errstat != 0)
			return errstat;
	}

	return 0;
}

enum gfreq_lib_errs convert_bits(
		struct byte_io *bio,
		const unsigned long *encrypt_table,
		const unsigned long *codes_len)
{
	enum gfreq_lib_errs errstat;
	unsigned char in_byte;
	unsigned char out_byte;
	unsigned long code;
	unsigned long j;
	unsigned long bit_index;
	unsigned long used_bits;

	if (bio == NULL || bio->in == NULL || bio->out == NULL ||
			encrypt_table == NULL || codes_len == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	rewind(bio->in);
	out_byte = 0;
	used_bits = 0;

	for (;;) {
		errstat = bio_read_uch(bio, &in_byte);
		if (errstat == gfreq_err_eof_file)
			break;
		if (errstat != 0)
			return errstat;

		code = encrypt_table[in_byte];
		for (j = 0; j < codes_len[in_byte]; j++) {
			bit_index = codes_len[in_byte] - j - 1;
			out_byte = (unsigned char) (out_byte << 1);
			out_byte = (unsigned char) (out_byte |
					((code >> bit_index) & 1UL));
			used_bits++;

			if (used_bits == 8) {
				errstat = bio_write_uch(bio, out_byte);
				if (errstat != 0)
					return errstat;

				out_byte = 0;
				used_bits = 0;
			}
		}
	}

	if (used_bits != 0) {
		out_byte = (unsigned char) (out_byte << (8 - used_bits));
		errstat = bio_write_uch(bio, out_byte);
		if (errstat != 0)
			return errstat;
	}

	return 0;
}

enum gfreq_lib_errs deconvert_bits(
		struct byte_io *bio,
		const struct huffman_tree *tree,
		unsigned long bits_len,
		unsigned long bytes_len)
{
	const struct node_ht *tnode;
	enum gfreq_lib_errs errstat;
	unsigned long i;
	unsigned char byte;

	if (bio == NULL || bio->in == NULL || bio->out == NULL ||
			tree == NULL || tree->root == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (tree->root->left == NULL && tree->root->right == NULL) {
		for (i = 0; i < bits_len; i++) {
			errstat = bio_write_uch(bio, tree->root->byte);
			if (errstat != 0)
				return errstat;
		}

		for (i = 0; i < bytes_len; i++) {
			errstat = bio_read_uch(bio, &byte);
			if (errstat != 0)
				return errstat;
		}

		return 0;
	}

	tnode = tree->root;
	byte = 0;

	for (i = 0; i < bits_len; i++) {
		if (i % 8 == 0) {
			errstat = bio_read_uch(bio, &byte);
			if (errstat != 0)
				return errstat;
		}

		if (byte & (1U << (7 - i % 8)))
			tnode = tnode->right;
		else
			tnode = tnode->left;

		if (tnode == NULL) {
			GFREQ_DEBUG_MSG;
			return gfreq_err_null_ptr;
		}

		if (tnode->left == NULL && tnode->right == NULL) {
			errstat = bio_write_uch(bio, tnode->byte);
			if (errstat != 0)
				return errstat;

			tnode = tree->root;
		}
	}

	return 0;
}
