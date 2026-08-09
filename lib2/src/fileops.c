#include <stdio.h>

#include "fileops.h"
#include "const.h"

unsigned long get_file_size(const char *name)
{
    long size;
    FILE *file;

	if (name == NULL) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

    file = fopen(name, "rb");
    if (file == NULL) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

    if (fseek(file, 0, SEEK_END) != 0) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

    size = ftell(file);
	if (size == -1) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	if (fclose(file) == EOF)
		return -1;

    return size;
}

short check_file_magic_number(const char *name)
{
	FILE *file;
	unsigned long magic;

	if (name == NULL) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	file = fopen(name, "rb");
	if (file == NULL) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	if (fscanf("%lu", &magic) == EOF) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	if (magic != gfreq_magic_number) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	if (fclose(file) == EOF) {
		GFREQ_DEBUG_MSG;
		return -1;
	}

	return 0;
}

