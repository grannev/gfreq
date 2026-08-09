/* debug.h */
#ifndef GFREQ_DEBUG_H
#define GFREQ_DEBUG_H

#ifdef GFREQ_LIB_DEBUG
#include <stdio.h>
#include "../include/errs.h"

	#define GFREQ_DEBUG_MSG \
		do { \
			fprintf(stderr, "gfreq: error at %s:%d\n, \
					__FILE__, __LINE__); \
		} while (0)
#else
	#define GFREQ_DEBUG_MSG ((void)0)
#endif

#endif
