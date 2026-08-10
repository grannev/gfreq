# gfreq

`gfreq` is an ANSI C library for file compression. It uses a skew heap to build
a Huffman tree and provides functions for packing and unpacking files.

CLI and GUI programs in this repository are examples of using the library from
C and Python[change for any other high-level lang].

## Requirements

* any ANSI C compatible compiler
* Makefile (ofc is optional)
* ar

## Build

Build the static library with GCC, GNU Make, and `ar`:
```sh
make lib
```
The result is `lib/libgfreq.a`. Public headers are in `lib/include`.

Build library and CLI program example of implementing
```sh
make
```
The result is `bin/gfreq`.

Dynamic library targets are also available:
```sh
make build_dynlib_linux
make build_dynlib_macos
make build_dynlib_win32
```
These targets produce `libgfreq.so`, `libgfreq.dylib`, or `gfreq.dll` in
`gui`.

## Public API

The main interface is declared in `lib/include/gfreq.h`:
```c
enum gfreq_lib_errs
gfreq_pack_file(const char *in_file_name, const char *out_file_name);

enum gfreq_lib_errs
gfreq_unpack_file(const char *in_file_name, const char *out_file_name);
```
Both functions return `0` on success or `enum gfreq_lib_errs` on failure.

Error helpers are declared in `lib/include/errs.h`:
```c
const char *gfreq_strerr(enum gfreq_lib_errs error);
void gfreq_printerr(enum gfreq_lib_errs error, FILE *stream);
```

Example:
```c
#include <stdio.h>

#include "gfreq.h"

int main(void)
{
	enum gfreq_lib_errs errstat;

	errstat = gfreq_pack_file("input.bin", "input.bin.ger");
	if (errstat != 0) {
		gfreq_printerr(errstat, stderr);
		return errstat;
	}

	return 0;
}
```

Compile against the static library:
```sh
gcc -ansi -pedantic -I lib/include example.c -L lib -lgfreq -lm -o example
```

## Implementation

The library counts byte frequencies and inserts the corresponding tree nodes
into a skew heap. It repeatedly extracts and merges the two lightest nodes
until the Huffman tree is complete. The resulting prefix codes are then used
to encode the input byte stream.

## Archive Format

A `.ger` archive contains:
```text
GFREQ magic number
frequency table
encoded bit count
encoded byte count
byte stream
```
The frequency table reconstructs the Huffman tree during unpacking. Numeric
header fields are decimal text, followed by the binary encoded stream.

## Example Clients

Build and use the command-line example:
```sh
make
./bin/gfreq pack input.bin
./bin/gfreq unpack input.bin.ger
```

The Python/Tkinter example loads the dynamic library through `ctypes`:
```sh
cd gui
python3 main.py
```
