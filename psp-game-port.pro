TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += SDL_VERS

SOURCES += \
    camera/camera.c \
    graphics/rectangle.c \
    graphics/text.c \
    input/input.c \
    map/lua/map_lua_functions.c \
    map/lua/map_lua_functions_input.c \
    map/lua/map_lua_functions_inventory.c \
    map/lua/map_lua_functions_message_box.c \
    map/lua/map_lua_functions_player.c \
    map/lua/map_lua_functions_sprite.c \
    map/tile.c \
    map/tilemap.c \
    message-box/message_box.c \
    player/inventory.c \
    player/player.c \
    serialization/serialization_reader.c \
    serialization/serializer.c \
    sound/sound.c \
    sprites/sprite.c \
    vector/vector.c \
    globals.c \
    main.c \
    system/system_info.c \
    level-editor/ui/tinyfiledialogs.c \
    level-editor/level-editor.c

CONFIG(debug, debug|release) {
    CONFIG += console
} else {
    CONFIG -= console
}

macx: {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -llua.5.2.4
}

win32: {
    ## For regular Win32
    #DEPENDPATH += $$PWD/dependencies/win32/lib
    #LIBS += -L$$PWD/dependencies/win32/lib -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -llua52
    #INCLUDEPATH += $$PWD/dependencies/win32/include

    ## For Mingw
    INCLUDEPATH += $$PWD/dependencies/win32_mingw/include
    #DEPENDPATH += $$PWD/dependencies/win32_mingw/lib
    #LIBS += -L$$PWD/dependencies/win32_mingw/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -llua52
    LIBS += -L"C:\Users\Mike\Documents\GitHub\psp-game\dependencies\win32_mingw\lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -llua52
}

DISTFILES += \
    map/level.bin \
    res/hack.pgf \
    res/ltn0.pgf \
    res/hack.ttf \
    res/text.ogg \
    res/text.wav \
    res/bg.png \
    res/bullet.png \
    res/edgelord.png \
    res/forest.png \
    res/inventory.png \
    res/items.png \
    res/ness.png \
    res/ness_lift.png \
    res/player.png \
    res/textures.png \
    res/beta/middle dirt mound.psd \
    res/beta/ness.psd \
    res/beta/ness_lift.psd \
    res/beta/pepper take two.psd \
    res/beta/playground.psd \
    res/beta/rock bottom.psd \
    res/beta/terribad pepper.psd \
    res/beta/top left dirt mound.psd \
    LICENSE \
    map/input.lua \
    map/inventory.lua \
    map/messagebox.lua \
    map/npc.lua \
    map/player.lua \
    map/script.lua \
    map/time.lua \
    README.md

HEADERS += \
    camera/camera.h \
    graphics/common.h \
    graphics/rectangle.h \
    graphics/text.h \
    input/input.h \
    map/lua/map_lua_functions.h \
    map/tile.h \
    map/tilemap.h \
    map/tilemap_lua.h \
    map/tilemap_serializer.h \
    message-box/message_box.h \
    player/inventory.h \
    player/player.h \
    serialization/serialization_reader.h \
    serialization/serializer.h \
    sound/sound.h \
    sprites/sprite.h \
    string/string.h \
    vector/vector.h \
    callback.h \
    common.h \
    globals.h \
    system/system_info.h \
    level-editor/ui/tinyfiledialogs.h \
    level-editor/level-editor.h
