CC = gcc
AR = ar

CFLAGS = -g -Wall -ansi -pedantic
ARFLAGS = -crs

SOURCES = *.c
OBJECTS = *.o

TARGET = libgfreq.a

static: all

all: clean build-obj pack-lib
	make clean-obj

pack-lib:
	$(AR) $(ARFLAGS) $(TARGET) $(OBJECTS)

build-obj:
	$(CC) $(CFLAGS) -c $(SOURCES)

clean: clean-obj clean-target

clean-obj:
	rm -f $(OBJECTS)

clean-target:
	rm -f $(TARGET)

