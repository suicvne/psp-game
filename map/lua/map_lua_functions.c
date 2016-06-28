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

int lua_map_rgba(lua_State* L)
{
  #ifdef PSP
  int arg_count = lua_gettop(L);
  int r, g, b, a;
  if(arg_count == 4)
    a = lua_tonumber(L, 4);
  else
    a = 255;

  r = lua_tonumber(L, 1);
  g = lua_tonumber(L, 2);
  b = lua_tonumber(L, 3);

  int color = RGBA(r, g, b, a);

  lua_pushnumber(L, color);
  return 1;
  #endif
  return 0;

  // TODO: fix weird alpha values?
  // int arg_count = lua_gettop(L);
  //
  // char r, g, b, a;
  //

  //
  // if(arg_count == 3) //RGB
  //   a = 0xff; //default
  // else if(arg_count == 4)
  //   a = lua_tonumber(L, 4);
  //
  // int final = ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24));
  // lua_pushnumber(L, final);
  // return 1;
}

int lua_map_draw_rect(lua_State* L)
{
  int arg_count = lua_gettop(L);
  if(arg_count < 5)
    return 0;

  int x0, x1, y0, y1, color;

  x0 = lua_tonumber(L, 1);
  y0 = lua_tonumber(L, 2);
  x1 = lua_tonumber(L, 3);
  y1 = lua_tonumber(L, 4);

  color = lua_tonumber(L, 5);

  #ifdef PSP
  oslDrawFillRect(x0, y0, x1, y1, color);
  //oslDrawFillRect(10, 10, 100, 100, RGB(0, 255, 0));
  #endif

  return 0;
}

int lua_map_set_blending(lua_State* L)
{
  #ifdef PSP
  if(lua_gettop(L) >= 1)
  {
    int argument = lua_tonumber(L, 1);
    int alpha_value = lua_tonumber(L, 2);

    oslSetAlpha(argument, alpha_value); //TODO: change
  }
  else
  {
    oslSetAlpha(OSL_FX_DEFAULT, 0);
  }
  #endif
  return 0;
}

int lua_map_test_blending(lua_State* L)
{
  #ifdef PSP
  int color = lua_tonumber(L, 1);
  oslSetAlpha(OSL_FX_ALPHA | OSL_FX_COLOR, color);
  #endif
  return 0;
}

int lua_map_register_functions(lua_State* L)
{
  lua_register(L, "print", lua_map_print);
  lua_register(L, "draw_text", lua_map_draw_text);
  lua_register(L, "RGBA", lua_map_rgba);
  lua_register(L, "draw_filled_rect", lua_map_draw_rect);
  lua_register(L, "draw_set_alpha", lua_map_set_blending);
  lua_register(L, "draw_set_alpha_color", lua_map_test_blending);
}

/*int lua_map_push_tilemap(lua_State* L, tilemap_t* map)
{

}*/
