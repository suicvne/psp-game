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

int lua_map_draw_text(lua_State* L)
{
  /*
  draw_text("text", x, y);
  */
  int arg_count = lua_gettop(L);
  if(arg_count < 3)
    return -1;

  int x, y;
  char* text;

  text = lua_tostring(L, 1);
  x = lua_tonumber(L, 2);
  y = lua_tonumber(L, 3);

  #ifdef PSP
  oslDrawStringf(x, y, text);
  #endif

  return 0;
}

int lua_map_register_functions(lua_State* L)
{
  lua_register(L, "print", lua_map_print);
  lua_register(L, "draw_text", lua_map_draw_text);
}

/*int lua_map_push_tilemap(lua_State* L, tilemap_t* map)
{

}*/
