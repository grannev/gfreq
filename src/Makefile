TARGET  = ../bin/gfreq
LIB     = ../lib/libgfreq.a

CFLAGS  = -g -Wall -ansi -pedantic -I ../lib
LDFLAGS = -L ../lib -lgfreq -lm

SOURCE  = *.c

static: all

all: clean dir-bin build-src

build-src:
	gcc $(CFLAGS) $(SOURCE) $(LDFLAGS) -o $(TARGET)

dir-bin:
	mkdir -p ../bin

clean:
	rm -rf bin

