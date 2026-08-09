
static: all

all: build_lib build_src

build_lib:
	cd lib && make

build_dynlib_linux:
	gcc -shared -fPIC -I ./lib/include -o ./gui/libgfreq.so ./lib/src/*.c

build_dynlib_win32:
	gcc -shared -I lib\include -o gui\gfreq.dll lib\src\*.c

build_dynlib_macos:
	clang -dynamiclib -I ./lib/include -o ./gui/libgfreq.dylib ./lib/src/*.c

build_src:
	cd src && make

clean:
	cd lib && make clean
	cd src && make clean
	rm -rf *.obj ./bin
