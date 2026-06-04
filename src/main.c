#include <string.h>
#include <stdio.h>

#include "errs.h"
#include "const.h"
#include "../lib/crypto.h"


int main(int argc, char **argv)
{
    const char *cmd;
    const char *file;
    unsigned long file_name_len;

    if (argc < 2) {
        printf(ERR_NO_CMD);
        printf(USAGE_MSG);
        return 1;
    }

    cmd = argv[1];

    if (strcmp(cmd, "compress") == 0) {
        if (argc < 3) {
            printf(ERR_NO_FILE);
            printf(USAGE_MSG);
            return 1;
        }       
        file = argv[2];
        compress(file);
        return 0;
    }

    if (strcmp(cmd, "decompress") == 0) {
        if (argc < 3) {
            printf(ERR_NO_FILE);
            printf(USAGE_MSG);
            return 1;
        }       
        file = argv[2];

        file_name_len = strlen(file);

        if (file_name_len <= 4) {
            printf(ERR_NO_GER);
            return 1;
        }

        if (strcmp(file + file_name_len - 4, ".ger") != 0) {
            printf(ERR_NO_GER);
            return 1;
        }

        decompress(file);
        return 0;
    }

    if (strcmp(cmd, "help") == 0   || 
        strcmp(cmd, "--help") == 0 || 
        strcmp(cmd, "-h") == 0) {
        printf(USAGE_MSG);
        return 0;
    }

    printf(ERR_UNK_CMD);
    printf(USAGE_MSG);

    return 4;
}
