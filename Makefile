#PSP MAKEFILE

TARGET = analogue
OBJS = main.o globals.o message-box/message_box.o graphics/text.o input/input.o camera/camera.o vector/vector.o player/player.o sprites/sprite.o serialization/serializer.o serialization/serialization_reader.o map/tilemap.o map/tile.o map/lua/map_lua_functions_player.o map/lua/map_lua_functions_input.o map/lua/map_lua_functions.o map/lua/map_lua_functions_message_box.o

INCDIR =
CFLAGS = -O2 -g -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti -DPSP
ASFLAGS = $(CFLAGS)

LIBDIR =
STDLIBS= -losl -lpng -lz -llua \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg
LIBS = $(STDLIBS)
LDFLAGS =

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Analogue Sample
PSP_EBOOT_ICON = ICON0.png
#PSP_EBOOT_PIC1 = PIC1.png

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
