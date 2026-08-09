#include "args.h"
#include "errs.h"

int main(int argc, char **argv)
{
	int err_stat;
	struct cli_arguments args;

	err_stat = resolve_arguments(&args, argc, argv);
	if (err_stat != 0) {
		printerr(err_stat);
		return err_stat;
	}

	err_stat = handle_arguments(&args);
	if (err_stat != 0) {
		printerr(err_stat);
		return err_stat;
	}

    return 0;
}

