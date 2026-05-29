TARGET = libgcmpr.a
SOURCE = *.c
FLAGS = -g -Wall -ansi -pedantic
macro = UNDEFINED

static: build

build:
	gcc $(FLAGS) $(SOURCE) -D $(macro) -o $(TARGET) -lm

