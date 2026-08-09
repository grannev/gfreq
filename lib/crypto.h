#ifndef CRYPTO_H
#define CRYPTO_H

/* #include "byte_rw.h" */

/* encrypt is ecnrypting data from in file to out file using byte_rw struct */
/* void encrypt(struct byte_rw *brw); */

/* dcerypt is decrypting data from in file to out file using byte_rw struct */
/* void decrypt(struct byte_rw *brw); */

/* compress is compressing file to file.ger, 
 * rewrites file.ger if already exist */
void compress(const char *in_file);

/* decompress is decompressing file.ger to file,
 * rewrites file if already exist */
void decompress(const char *in_file);

#endif

