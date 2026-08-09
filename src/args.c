#include <string.h>
#include <stdio.h>

#include "../lib/crypto.h"
#include "const.h"
#include "args.h"
#include "errs.h"

int handle_arguments(struct cli_arguments *args)
{
	int cmp_stat/*, err_stat*/;

	cmp_stat = strcmp(args->command, "compress") == 0;
	if (cmp_stat) {
		compress(args->file_name);
		/* err_stat = compress_file(args->file_name); */
		/* if (err_stat != 0) */
		/* 	return err_stat; */
		return 0;
	}
	
	cmp_stat = strcmp(args->command, "decompress") == 0;
	if (cmp_stat) {
		decompress(args->file_name);
		/* err_stat = decompress_file(args->file_name); */
		/* if (err_stat != 0) */
		/* 	return err_stat; */
		return 0;
	}

	cmp_stat = strcmp(args->command, "--help") == 0 ||
			   strcmp(args->command, "help") == 0 ||
			   strcmp(args->command, "-h") == 0;
	if (cmp_stat) {
		puts(USAGE_MSG);
		return 0;
	}

	puts(USAGE_MSG);
	return ERR_UNK_CMD;
}

int resolve_arguments(
		struct cli_arguments *args,
		int argc,
		char **argv)
{
	if (argc < 2)
		return ERR_NO_CMD;
	args->command = argv[1];
	
	if (argc < 3) {
		args->file_name = "";
		args->file_name_len = 0;
	} else {
		args->file_name = argv[2];
		args->file_name_len = strlen(argv[2]);
	}
	
	return 0;
}

