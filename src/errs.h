#ifndef GFREQ_CLI_ERRS_H
#define GFREQ_CLI_ERRS_H

enum cli_errors {
	ERR_NO_CMD  = 1,
	ERR_UNK_CMD = 2,
	ERR_NO_FILE = 3,
	ERR_NO_GER  = 4
};

void printerr(int error);

#endif
