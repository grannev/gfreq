#ifndef FILEOPS_H
#define FILEOPS_H

#include "../include/errs.h"

/* -1 : there some sort of error */
unsigned long get_file_size(const char *name);

/* -1 : file has no magic number */
/*  0 : file has magic number */ 
short check_file_magic_number(const char *name);

#endif
