#include "map_lua_functions.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "../../input/input.h"

int lua_input_is_button_pressed(lua_State* L)
{
  int arg_count = lua_gettop(L);
  if(arg_count != 1)
    return 0;

  int button_id = -1;
  button_id = lua_tonumber(L, 1);

  lua_pushnumber(L, input_is_button_just_pressed(button_id));

  return 1;
}
