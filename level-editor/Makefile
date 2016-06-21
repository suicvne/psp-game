PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags sdl2) -DSDL_VERS
LIBS = $(shell $(PKGCONFIG) --libs sdl2) -lm

compile:
	gcc $(CFLAGS) -Wall src/*.c -o gen/level-editor $(LIBS)
run:
	gen/level-editor
