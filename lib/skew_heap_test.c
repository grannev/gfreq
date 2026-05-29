#include "crypto.h"
/* #include "ubuffer.h" */


int main(int argc, char **argv)
{
    /* ubuffer str; */

    /* init_ubuf(&str, ""); */
    /* add_ubuf(&str, ". Hello, World!"); */
    /* print_ubuf(&str); */

    if (argc < 3)
        return 1;

    /* cstring = encrypt(argv[1]); */
    /* puts(cstring); */
    /* free(cstring); */

    archive(argv[2], argv[1]);

    /* free_ubuf(&str); */


    return 0;
}
