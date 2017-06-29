# Makefile template for shared library

OS = meme

#LUA = false

ifeq ($(OS),Windows_NT)
	OS = WIN32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OS = LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OS = OSX
	endif
endif

CC = gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g # C flags
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command

TARGET_LIB = ../libs/librocklevel.so
PKGCONFIG = $(shell which pkg-config)

ifeq ($(LUA),true)
# 	Need to compile with Lua, probably for the actual game.
	CFLAGS += $(shell $(PKGCONFIG) --cflags lua5.2) -DLUA
	LDFLAGS += $(shell $(PKGCONFIG) --libs lua5.2)
endif

ifeq ($(OS),OSX)
	TARGET_LIB = ../libs/librocklevel.dylib
#	echo "OS X Target"
endif

ifeq ($(OS),WIN32)
	TARGET_LIB = ../libs/librocklevel.dll
#	echo "WIN32 Target"
endif

SRCS = rocklevel/camera.c rocklevel/serialization_reader.c rocklevel/serializer.c rocklevel/tile.c rocklevel/tilemap.c rocklevel/vector.c # source files
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d)
