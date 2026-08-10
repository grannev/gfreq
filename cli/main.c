#include <stdio.h>

#include "../lib/include/gfreq.h"
#include "const.h"
#include "args.h"
#include "errs.h"

int main(int argc, char **argv)
{
	enum cli_errors cli_errstat;
	enum gfreq_lib_errs lib_errstat;
	struct cli_arguments args;

	cli_errstat = resolve_arguments(&args, argc, argv);
	if (cli_errstat != 0) {
		printerr(cli_errstat, stderr);
		return cli_errstat;
	}

	if (args.action == action_help) {
		puts(usage_msg);
		return 0;
	}

	if (args.action == action_pack)
		lib_errstat = gfreq_pack_file(args.in_file_name, args.out_file_name);
	else
		lib_errstat = gfreq_unpack_file(args.in_file_name, args.out_file_name);
	free_arguments(&args);

	if (lib_errstat != 0) {
		gfreq_printerr(lib_errstat, stderr);
		return lib_errstat;
	}

	return 0;
}
