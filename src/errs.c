#include "errs.h"

static const char err_no_cmd_msg[] = "command required";
static const char err_unk_cmd_msg[] = "unknown command";
static const char err_no_file_msg[] = "file path is required";
static const char err_no_ger_msg[] = "file extension must be .ger";
static const char err_no_memory_msg[] = "memory allocation failed";
static const char err_unknown_msg[] = "unknown client error";

void printerr(enum cli_errors error, FILE *stream)
{
	fprintf(stream, "Error: ");
	switch (error) {
		case err_no_cmd:
			fputs(err_no_cmd_msg, stream); break;
		case err_no_ger:
			fputs(err_no_ger_msg, stream); break;
		case err_unk_cmd:
			fputs(err_unk_cmd_msg, stream); break;
		case err_no_file:
			fputs(err_no_file_msg, stream); break;
		case err_no_memory:
			fputs(err_no_memory_msg, stream); break;
		default:
			fputs(err_unknown_msg, stream); break;
	}
	fputs("\n", stream);
}
