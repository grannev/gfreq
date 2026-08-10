#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "errs.h"

static int has_ger_extension(
		const char *file_name,
		unsigned long file_name_len)
{
	if (file_name_len <= 4)
		return 0;

	return strcmp(file_name + file_name_len - 4, ".ger") == 0;
}

static enum cli_errors make_packed_name(
		const char *file_name,
		unsigned long file_name_len,
		char **out_file_name)
{
	*out_file_name = malloc(file_name_len + 5);
	if (*out_file_name == NULL)
		return err_no_memory;

	strcpy(*out_file_name, file_name);
	strcat(*out_file_name, ".ger");

	return 0;
}

static enum cli_errors make_unpacked_name(
		const char *file_name,
		unsigned long file_name_len,
		char **out_file_name)
{
	unsigned long out_file_name_len;

	if (!has_ger_extension(file_name, file_name_len))
		return err_no_ger;

	out_file_name_len = file_name_len - 4;
	*out_file_name = malloc(out_file_name_len + 1);
	if (*out_file_name == NULL)
		return err_no_memory;

	strncpy(*out_file_name, file_name, out_file_name_len);
	(*out_file_name)[out_file_name_len] = '\0';

	return 0;
}

enum cli_errors resolve_arguments(
		struct cli_arguments *args,
		int argc,
		char **argv)
{
	unsigned long file_name_len;

	args->action = action_help;
	args->in_file_name = NULL;
	args->out_file_name = NULL;

	if (argc < 2)
		return err_no_cmd;

	if (strcmp(argv[1], "--help") == 0 ||
			strcmp(argv[1], "help") == 0 ||
			strcmp(argv[1], "-h") == 0) {
		args->action = action_help;
		return 0;
	}

	if (argc < 3)
		return err_no_file;

	args->in_file_name = argv[2];
	file_name_len = strlen(args->in_file_name);

	if (strcmp(argv[1], "pack") == 0) {
		args->action = action_pack;
		return make_packed_name(
				args->in_file_name,
				file_name_len,
				&args->out_file_name);
	}

	if (strcmp(argv[1], "unpack") == 0) {
		args->action = action_unpack;
		return make_unpacked_name(
				args->in_file_name,
				file_name_len,
				&args->out_file_name);
	}

	return err_unk_cmd;
}

void free_arguments(struct cli_arguments *args)
{
	if (args == NULL)
		return;

	free(args->out_file_name);
	args->out_file_name = NULL;
}
