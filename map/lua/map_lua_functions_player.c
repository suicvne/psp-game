/**
Used for the player specific functions in Lua.
*/

#include "map_lua_functions.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "../../globals.h"
#include "../../vector/vector.h"
#include "../../camera/camera.h"
#include "../../sprites/sprite.h"

#include "../../player/player.h"

int lua_player_get_x(lua_State* L)
{
  vector_t transformed_coordinates = camera_player_to_world(kCamera);
  lua_pushnumber(L, transformed_coordinates.x);
  return 1;
}

int lua_player_get_y(lua_State* L)
{
  vector_t transformed_coordinates = camera_player_to_world(kCamera);
  lua_pushnumber(L, transformed_coordinates.y);
  return 1;
}

int lua_player_get_direction(lua_State* L)
{
  lua_pushnumber(L, kPlayer->main_sprite->yframeoffset);
  return 1;
}
