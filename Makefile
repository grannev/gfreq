
static: all

all: build-lib build-src

build-lib:
	cd lib && make

build-dynlib-linux:
	gcc -shared -fPIC -o ./gui/libgfreq.so ./lib/*.c

build-dynlib-win32:
	gcc -shared -o gui\gfreq.dll lib\*.c

build-dynlib-macos:
	clang -dynamiclib -o libgfreq.dylib ./lib/*.c

build-src:
	cd src && make

clean:
	rm -rf *.obj ./bin

