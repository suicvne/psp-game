/**

Defines common functions for the map's Lua interpreter.

*/

#ifndef ___MAP_LUA_FUNCTIONS_H___
#define ___MAP_LUA_FUNCTIONS_H___

#include "../tilemap.h"
#ifdef PSP
#include <oslib/oslib.h>
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int lua_map_print(lua_State* L);
int lua_map_draw_text(lua_State* L);
int lua_map_register_functions(lua_State* L);

//int lua_map_push_tilemap(lua_State* L, tilemap_t* map);

#endif //___MAP_LUA_FUNCTIONS_H___
