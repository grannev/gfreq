#include <stdlib.h>

#include "byte_rw.h"
#include "fileops.h"
#include "errs.h"

void brw_init(
        struct byte_rw *brw,
        const char *in_name,
        const char *out_name
) {
    if (brw == NULL) {
        printf(ERR_INIT_NULL);
        exit(ERR_BYTE_RW);
    }

    brw->in_len = get_file_size(in_name);
    brw->out_len = get_file_size(out_name);

    brw->in = fopen(in_name, "rb");
    brw->out = fopen(out_name, "wb");
    if (brw->in == NULL || brw->out == NULL) {
        printf(ERR_OPEN_FILE);
        exit(ERR_BYTE_RW);
    }
    
    brw->in_eof = 0;
}

unsigned char brw_next(struct byte_rw *brw)
{
    short input;
    
    if (brw == NULL) {
        printf(ERR_NEXT_NULL);
        exit(ERR_BYTE_RW);
    }
    
    input = fgetc(brw->in);
    if (input == EOF) {
        brw->in_eof = 1;
        return 0;
    }

    return (unsigned char) input;
}

unsigned long brw_next_ulong(struct byte_rw *brw)
{
    unsigned long result;
    
    if (brw == NULL) {
        printf(ERR_NEXT_NULL);
        exit(ERR_BYTE_RW);
    }

    fscanf(brw->in, "%lu", &result);
    return result;
}

void brw_put_uch(struct byte_rw *brw, unsigned char uch)
{
    if (brw == NULL) {
        printf(ERR_PUT_NULL);
        exit(ERR_BYTE_RW);
    }
    
    fputc(uch, brw->out);
}

void brw_put_ulong(struct byte_rw *brw, unsigned long value)
{
    if (brw == NULL) {
        printf(ERR_PUT_NULL);
        exit(ERR_BYTE_RW);
    }
    
    fprintf(brw->out, "%lu", value);
}

void brw_close(struct byte_rw *brw)
{
    if (brw == NULL) {
        printf(ERR_CLOSE_NULL);
        exit(ERR_BYTE_RW);
    }

    if (brw->in != NULL)
        fclose(brw->in);
    if (brw->out != NULL)
        fclose(brw->out);
}

