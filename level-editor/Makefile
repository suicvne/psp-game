PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags sdl2) -DSDL_VERS
LIBS = $(shell $(PKGCONFIG) --libs sdl2) -lm

LOCAL_DEPS = src/map/*.c src/player/*.c src/serialization/*.c src/sprites/*.c src/vector/*.c

compile:
	gcc $(CFLAGS) -Wall src/*.c $(LOCAL_DEPS) -o gen/level-editor $(LIBS)
run:
	gen/level-editor
clean:
	rm -rf src/*.o src/map/*.o src/player/*.o src/serialization/*.o src/sprites/*.o src/vector/*.o
	rm gen/level-editor
