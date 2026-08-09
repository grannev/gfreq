TARGET  = ../bin/gfreq
LIB     = ../lib/libgfreq.a

CFLAGS  = -g -Wall -ansi -pedantic -I ../lib/include
LDFLAGS = -L ../lib -lgfreq -lm

SOURCE  = *.c

static: all

all: clean dir_bin build_src

build_src:
	gcc $(CFLAGS) $(SOURCE) $(LDFLAGS) -o $(TARGET)

dir_bin:
	mkdir -p ../bin

clean:
	rm -rf ../bin
