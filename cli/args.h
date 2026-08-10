#ifndef GFREQ_CLI_ARGS_H
#define GFREQ_CLI_ARGS_H

#include "errs.h"

enum cli_action {
	action_help,
	action_pack,
	action_unpack
};

struct cli_arguments {
	enum cli_action action;
	const char *in_file_name;
	char *out_file_name;
};

enum cli_errors resolve_arguments(
		struct cli_arguments *args,
		int argc,
		char **argv);

void free_arguments(struct cli_arguments *args);

#endif
