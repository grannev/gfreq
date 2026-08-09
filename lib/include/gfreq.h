#ifndef GFREQ_LIB_GFREQ_H
#define GFREQ_LIB_GFREQ_H

#include "errs.h"

enum gfreq_lib_errs
gfreq_pack_file(const char *in_file_name, const char *out_file_name);

enum gfreq_lib_errs
gfreq_unpack_file(const char *in_file_name, const char *out_file_name);

#endif
