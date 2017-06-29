#PSP MAKEFILE

TARGET = mini-rust
OBJS = main.o sound/sound.o globals.o camera/camera.o player/inventory.o graphics/rectangle.o message-box/message_box.o graphics/text.o input/input.o vector/vector.o player/player.o sprites/sprite.o serialization/serializer.o serialization/serialization_reader.o map/tilemap.o map/tile.o  map/lua/map_lua_functions_inventory.o map/lua/map_lua_functions_player.o map/lua/map_lua_functions_sprite.o map/lua/map_lua_functions_input.o map/lua/map_lua_functions.o map/lua/map_lua_functions_message_box.o

INCDIR =
CFLAGS = -O2 -g -G0 -Wall -DLUA_32BITS
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -DPSP
ASFLAGS = $(CFLAGS)

LIBDIR =
STDLIBS= -losl -lpng -lz -llua \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg
LIBS = $(STDLIBS)
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Mini Rust
PSP_EBOOT_ICON = ICON0.png
#PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
