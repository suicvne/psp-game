#include "map_lua_functions.h"

int lua_map_print(lua_State* L)
{
  int arg_count = lua_gettop(L);
  int i;

  for(i = 1; i < arg_count + 1; i++) //Lua is not 0 based
  {
    printf("Lua: %s\n", lua_tostring(L, i));
  }
  return 0;
}
