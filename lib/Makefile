CC = gcc
AR = ar

CFLAGS = -g -Wall -Wextra -ansi -pedantic -I include -I src
ARFLAGS = -crs

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = libgfreq.a

all: static

static: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: clean_obj clean_target

clean_obj:
	rm -f src/*.o *.o

clean_target:
	rm -f $(TARGET)
