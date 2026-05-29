TARGET = gerazip
SOURCE = ./src/*
CFLAGS = -g -Wall -ansi -pedantic -L:./lib/libcrypto.a

LIBTRG = libcrypto.a
LIBSRC = ./lib/*
LFLAGS = -g -Wall -ansi -pedantic

build-src:
	gcc $(CFLAGS) $(SOURCE) -o ./bin/$(TARGET)

build-lib:
	gcc $(LFLAGS) $(LIBSRC) -o ./lib/$(LIBTRG)

