#include <stdio.h>

#include "errs.h"

/* #define ERR_NO_CMD "Error: command required\n" */
/* #define ERR_UNK_CMD "Error: unknown command\n" */
/* #define ERR_NO_FILE "Error: file path is required\n" */
/* #define ERR_NO_GER "Error: file extension must be .ger\n" */

#define ERR_NO_CMD_MSG  "command required"
#define ERR_UNK_CMD_MSG "unknown command"
#define ERR_NO_FILE_MSG "file path is required"
#define ERR_NO_GER_MSG  "file extension must be .ger"
#define ERR_UNKNOWN_MSG "unknown error"

void printerr(int error)
{
	fputs("Error: ", stdout);
	switch (error) {
		case ERR_NO_CMD:  puts(ERR_NO_CMD_MSG);  break;
		case ERR_NO_GER:  puts(ERR_NO_GER_MSG);  break;
		case ERR_UNK_CMD: puts(ERR_UNK_CMD_MSG); break;
		case ERR_NO_FILE: puts(ERR_NO_FILE_MSG); break;
		default: 		  puts(ERR_UNKNOWN_MSG); break;
	}
}

