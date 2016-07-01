/**

Defines common functions for the map's Lua interpreter.

*/

#ifndef ___MAP_LUA_FUNCTIONS_H___
#define ___MAP_LUA_FUNCTIONS_H___

#include "../tilemap.h"
#include "../../graphics/text.h"
#include "../../player/player.h"
#include "../../vector/vector.h"
#include "../../camera/camera.h"
#include "../../globals.h"

#ifdef PSP
#include <oslib/oslib.h>
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
Drawing functions
*/
int lua_map_print(lua_State* L);
int lua_map_draw_text(lua_State* L);
int lua_map_rgba(lua_State* L);
int lua_map_register_functions(lua_State* L, struct tilemap_t* tilemap);
int lua_map_draw_rect(lua_State* L);
int lua_map_draw_rect_camera(lua_State* L);

int lua_map_set_blending(lua_State* L);
int lua_map_test_blending(lua_State* L);
/**
End Drawing functions
*/

/**
Tilemap functions - map_lua_functions.c
*/

int lua_map_get_name(lua_State* L);
int lua_map_get_width(lua_State* L);
int lua_map_get_height(lua_State* L);

int lua_map_set_tile(lua_State* L);

/**
End Tilemap functions
*/

/**
Player functions - map_lua_functions_player.c
*/

//Retrieve the player's X transformed with Camera coordinates
int lua_player_get_x(lua_State* L);

//Retrieve the player's Y transformed with Camera coordinates
int lua_player_get_y(lua_State* L);

//Retrieve the player's current direction based on its y-offset
int lua_player_get_direction(lua_State* L);

/**
End Player Functions
*/

/**
Input Functions - map_lua_functions_input.c
*/

int lua_input_is_button_pressed(lua_State* L);

/**
End Input Functions
*/

#endif //___MAP_LUA_FUNCTIONS_H___
