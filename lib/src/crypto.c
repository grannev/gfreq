#include <stdlib.h>

#include "crypto.h"
#include "convert.h"
#include "debug.h"

enum gfreq_lib_errs
cm_init(
		struct crypto_module *cm,
		const char *in_file_name,
		const char *out_file_name)
{
	enum gfreq_lib_errs errstat;
	unsigned long i;

	if (cm == NULL || in_file_name == NULL || out_file_name == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	cm->bio = NULL;
	cm->heap = NULL;
	cm->tree = NULL;

	cm->bio = malloc(sizeof(struct byte_io));
	if (cm->bio == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_no_memory;
	}

	cm->bio->in = NULL;
	cm->bio->out = NULL;

	cm->heap = malloc(sizeof(struct skew_heap));
	if (cm->heap == NULL) {
		GFREQ_DEBUG_MSG;
		cm_finalize(cm);
		return gfreq_err_no_memory;
	}

	errstat = sh_init(cm->heap);
	if (errstat != 0) {
		cm_finalize(cm);
		return errstat;
	}

	cm->tree = malloc(sizeof(struct huffman_tree));
	if (cm->tree == NULL) {
		GFREQ_DEBUG_MSG;
		cm_finalize(cm);
		return gfreq_err_no_memory;
	}

	errstat = ht_init(cm->tree);
	if (errstat != 0) {
		cm_finalize(cm);
		return errstat;
	}

	errstat = bio_open(cm->bio, in_file_name, out_file_name);
	if (errstat != 0) {
		cm_finalize(cm);
		return errstat;
	}

	for (i = 0; i < byte_cnt; i++) {
		cm->cryptable[i] = 0;
		cm->frequency[i] = 0;
		cm->codes_len[i] = 0;
	}

	cm->bits = 0;
	cm->bytes = 0;

	return 0;
}

enum gfreq_lib_errs
cm_finalize(struct crypto_module *cm)
{
	enum gfreq_lib_errs errstat;
	enum gfreq_lib_errs current_errstat;

	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = 0;
	if (cm->bio != NULL) {
		current_errstat = 0;
		if (cm->bio->in != NULL && cm->bio->out != NULL) {
			current_errstat = bio_close(cm->bio);
		} else {
			if (cm->bio->in != NULL && fclose(cm->bio->in) == EOF)
				current_errstat = gfreq_err_eof_file;
			if (cm->bio->out != NULL && fclose(cm->bio->out) == EOF)
				current_errstat = gfreq_err_eof_file;
		}
		if (current_errstat != 0)
			errstat = current_errstat;
	}

	if (cm->heap != NULL) {
		current_errstat = sh_free(cm->heap);
		if (errstat == 0 && current_errstat != 0)
			errstat = current_errstat;
	}

	if (cm->tree != NULL) {
		current_errstat = ht_free(cm->tree);
		if (errstat == 0 && current_errstat != 0)
			errstat = current_errstat;
	}

	free(cm->bio);
	free(cm->heap);
	free(cm->tree);
	cm->bio = NULL;
	cm->heap = NULL;
	cm->tree = NULL;

	if (errstat != 0)
		return errstat;

	return 0;
}

static enum gfreq_lib_errs build_frequency(struct crypto_module *cm)
{
	enum gfreq_lib_errs errstat;
	unsigned char byte;

	if (cm == NULL || cm->bio == NULL || cm->bio->in == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	for (;;) {
		errstat = bio_read_uch(cm->bio, &byte);
		if (errstat == gfreq_err_eof_file)
			break;
		if (errstat != 0)
			return errstat;

		cm->frequency[byte]++;
	}

	rewind(cm->bio->in);
	return 0;
}

static enum gfreq_lib_errs build_heap(struct crypto_module *cm)
{
	enum gfreq_lib_errs errstat;
	unsigned long i;

	if (cm == NULL || cm->heap == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	for (i = 0; i < byte_cnt; i++) {
		if (cm->frequency[i] == 0)
			continue;
		errstat = sh_insert(cm->heap, (unsigned char) i, cm->frequency[i]);
		if (errstat != 0)
			return errstat;
	}

	return 0;
}

static enum gfreq_lib_errs build_tree(struct crypto_module *cm)
{
	struct node_ht *left;
	struct node_ht *right;
	struct node_ht *parent;
	enum gfreq_lib_errs errstat;

	if (cm == NULL || cm->heap == NULL || cm->tree == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (cm->heap->size == 0)
		return 0;

	while (cm->heap->size > 1) {
		parent = malloc(sizeof(struct node_ht));
		if (parent == NULL) {
			GFREQ_DEBUG_MSG;
			return gfreq_err_no_memory;
		}

		errstat = sh_pop(cm->heap, &left);
		if (errstat != 0) {
			free(parent);
			return errstat;
		}

		errstat = sh_pop(cm->heap, &right);
		if (errstat != 0) {
			node_ht_free(left);
			free(parent);
			return errstat;
		}

		errstat = node_ht_init(parent, 0, left->weight + right->weight);
		if (errstat != 0) {
			node_ht_free(left);
			node_ht_free(right);
			free(parent);
			return errstat;
		}

		errstat = ht_insert(cm->tree, left, right, parent);
		if (errstat != 0) {
			node_ht_free(left);
			node_ht_free(right);
			free(parent);
			return errstat;
		}

		errstat = sh_insert_node(cm->heap, parent);
		if (errstat != 0)
			return errstat;

		cm->tree->root = NULL;
	}

	errstat = sh_pop(cm->heap, &cm->tree->root);
	if (errstat != 0)
		return errstat;

	if (cm->tree->size == 0)
		cm->tree->size = 1;

	return 0;
}

static enum gfreq_lib_errs build_code(
		struct crypto_module *cm,
		const struct node_ht *node,
		unsigned long code,
		unsigned long code_len)
{
	enum gfreq_lib_errs errstat;

	if (cm == NULL || node == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (node->left == NULL && node->right == NULL) {
		cm->cryptable[node->byte] = code;
		cm->codes_len[node->byte] = code_len == 0 ? 1 : code_len;
		return 0;
	}

	errstat = build_code(cm, node->left, code << 1, code_len + 1);
	if (errstat != 0)
		return errstat;

	errstat = build_code(cm, node->right, (code << 1) | 1UL, code_len + 1);
	if (errstat != 0)
		return errstat;

	return 0;
}

static enum gfreq_lib_errs build_cryptable(struct crypto_module *cm)
{
	if (cm == NULL || cm->tree == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	if (cm->tree->root == NULL)
		return 0;

	return build_code(cm, cm->tree->root, 0, 0);
}

static enum gfreq_lib_errs count_encrypt_data(struct crypto_module *cm)
{
	unsigned long i;

	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	cm->bits = 0;
	for (i = 0; i < byte_cnt; i++)
		cm->bits += cm->frequency[i] * cm->codes_len[i];
	cm->bytes = (cm->bits + 7) / 8;

	return 0;
}

enum gfreq_lib_errs
encrypt(struct crypto_module *cm)
{
	enum gfreq_lib_errs errstat;

	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = build_frequency(cm);
	if (errstat != 0)
		return errstat;

	errstat = build_heap(cm);
	if (errstat != 0)
		return errstat;

	errstat = build_tree(cm);
	if (errstat != 0)
		return errstat;

	errstat = build_cryptable(cm);
	if (errstat != 0)
		return errstat;

	errstat = count_encrypt_data(cm);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_ulong(cm->bio, gfreq_magic_number);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_uch(cm->bio, '\n');
	if (errstat != 0)
		return errstat;

	errstat = convert_table(cm->bio, cm->frequency);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_ulong(cm->bio, cm->bits);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_uch(cm->bio, '\n');
	if (errstat != 0)
		return errstat;

	errstat = bio_write_ulong(cm->bio, cm->bytes);
	if (errstat != 0)
		return errstat;

	errstat = bio_write_uch(cm->bio, '\n');
	if (errstat != 0)
		return errstat;

	errstat = convert_bits(cm->bio, cm->cryptable, cm->codes_len);
	if (errstat != 0)
		return errstat;

	return 0;
}

enum gfreq_lib_errs
decrypt(struct crypto_module *cm)
{
	enum gfreq_lib_errs errstat;
	unsigned long magic;
	unsigned char separator;

	if (cm == NULL) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_null_ptr;
	}

	errstat = bio_read_ulong(cm->bio, &magic);
	if (errstat != 0) return errstat;

	if (magic != gfreq_magic_number) {
		GFREQ_DEBUG_MSG;
		return gfreq_err_no_magic;
	}

	errstat = bio_read_uch(cm->bio, &separator);
	if (errstat != 0) return errstat;

	errstat = deconvert_table(cm->bio, cm->frequency);
	if (errstat != 0) return errstat;

	errstat = build_heap(cm);
	if (errstat != 0) return errstat;

	errstat = build_tree(cm);
	if (errstat != 0) return errstat;

	errstat = bio_read_ulong(cm->bio, &cm->bits);
	if (errstat != 0) return errstat;

	errstat = bio_read_uch(cm->bio, &separator);
	if (errstat != 0) return errstat;

	errstat = bio_read_ulong(cm->bio, &cm->bytes);
	if (errstat != 0) return errstat;

	errstat = bio_read_uch(cm->bio, &separator);
	if (errstat != 0) return errstat;

	if (cm->bits == 0) return 0;

	errstat = deconvert_bits(cm->bio, cm->tree, cm->bits, cm->bytes);
	if (errstat != 0) return errstat;

	return 0;
}
