#ifndef GFREQ_CLI_ARGS_H
#define GFREQ_CLI_ARGS_H

struct cli_arguments {
	const char *command;
	const char *file_name;
	unsigned long file_name_len;
};

int handle_arguments(struct cli_arguments *args);

int resolve_arguments(
		struct cli_arguments *args,
		int argc,
		char **argv);

#endif

