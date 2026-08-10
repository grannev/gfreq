#ifndef GFREQ_CLI_ERRS_H
#define GFREQ_CLI_ERRS_H

#include <stdio.h>

enum cli_errors {
	err_no_cmd  = 1,
	err_unk_cmd = 2,
	err_no_file = 3,
	err_no_ger  = 4,
	err_no_memory = 5
};

void printerr(enum cli_errors error, FILE *stream);

#endif
