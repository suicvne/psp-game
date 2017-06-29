#include "map_lua_functions.h"

#include "../tilemap.h"

/*
#include "../../sprites/sprite.h"
#include "../../graphics/rectangle.h"
#include "../../graphics/text.h"
#include "../../player/player.h"
#include "../../player/inventory.h"
#include "../../vector/vector.h"
#include "../../camera/camera.h"
#include "../../globals.h"
#include "../../message-box/message_box.h"
*/

lua_State* current_lua_state;

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
  const char* text;

  text = lua_tostring(L, 1);
  x = lua_tonumber(L, 2);
  y = lua_tonumber(L, 3);

  text_render_text(text, x, y);

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
  #elif SDL_VERS
  SDL_Rect rect = {x0, y0, x1, y1};
  SDL_RenderFillRect(kSdlRenderer, &rect);
  #endif

  return 0;
}

int lua_map_draw_rect_camera(lua_State* L)
{
  int arg_count = lua_gettop(L);
  if(arg_count < 5)
    return 0;

  int x0, x1, y0, y1, color;

  x0 = lua_tonumber(L, 1);
  y0 = lua_tonumber(L, 2);
  x1 = lua_tonumber(L, 3);
  y1 = lua_tonumber(L, 4);

  rectangle_t rectangle;
  rectangle.x = x0;
  rectangle.y = y0;
  rectangle.w = x1 - x0;
  rectangle.h = y1 - y0;

  color = lua_tonumber(L, 5);

  rectangle_draw_camera(&rectangle, color, kCamera);

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

int lua_map_draw_sprite(lua_State* L)
{
  //Should just pass the sprite pointer since x & y, etc. are handled by the sprite's rectangle parameter
  int arg_count = lua_gettop(L);
  if(arg_count == 1)
  {
    sprite_t* sprite = ((sprite_t*)lua_touserdata(L, 1));
    if(sprite != NULL)
    {
      sprite_draw_camera(sprite, *kCamera);
      return 0;
    }
  }
  return 0;
}

void lua_map_register_functions(lua_State* L, tilemap_t* tilemap)
{
  current_lua_state = L;
  
/**
  Setting path properly
  */
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "path");
  const char* cur_path = lua_tostring(L, -1);
  printf("lua cur_path: %s\n", cur_path);
  /*const char* to_append = ";./map/?.lua"; //what to append

  int combined_size = sizeof(char) * (strlen(cur_path) + strlen(to_append)) + 1;
  char* combined_path = malloc(combined_size);
  memset(combined_path, 0, combined_size);
  strcat(combined_path, cur_path);
  strcat(combined_path, to_append);
  */
  char* new_path = "./res/lua/?.lua";
  printf("lua path is now %s\n", new_path);
  lua_pop(L, 1);
  lua_pushstring(L, new_path);
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);

  /**
  End Setting Path Properly
  */

  printf("pushing tilemap to lua..\n");
  lua_pushlightuserdata(L, (void*)tilemap);
  lua_setglobal(L, "_current_tilemap");

  //TODO: rename these "general" functions

  printf("registering functions for lua..\n");
  lua_register(L, "print", lua_map_print);
  lua_register(L, "draw_text", lua_map_draw_text);
  lua_register(L, "RGBA", lua_map_rgba);
  lua_register(L, "draw_rect", lua_map_draw_rect);
  lua_register(L, "draw_rect_camera", lua_map_draw_rect_camera);
  lua_register(L, "draw_set_alpha", lua_map_set_blending);
  lua_register(L, "draw_set_alpha_color", lua_map_test_blending);

  lua_register(L, "map_draw_sprite", lua_map_draw_sprite);

  //player
  lua_register(L, "player_get_x", lua_player_get_x);
  lua_register(L, "player_get_y", lua_player_get_y);
  lua_register(L, "player_get_direction", lua_player_get_direction);
  //end player

  //tilemap
  lua_register(L, "tilemap_get_name", lua_map_get_name);
  lua_register(L, "tilemap_get_width", lua_map_get_width);
  lua_register(L, "tilemap_get_height", lua_map_get_height);
  lua_register(L, "tilemap_set_tile", lua_map_set_tile);
  lua_register(L, "tilemap_get_tile", lua_map_get_tile);
  //end tilemap

  //input
  lua_register(L, "input_is_button_down", lua_input_is_button_pressed);
  //end input

  //message box
  lua_register(L, "message_box_set_visibility", lua_message_box_set_visible);
  lua_register(L, "message_box_is_visible", lua_message_box_is_visible);
  lua_register(L, "message_box_set_message", lua_message_box_set_message);
  lua_register(L, "message_box_get_message", lua_message_box_get_message);
  lua_register(L, "message_box_draw", lua_message_box_draw);
  lua_register(L, "message_box_update", lua_message_box_update);
  //end message box

  //sprites
  lua_register(L, "sprite_create", lua_sprite_create);
  lua_register(L, "sprite_destroy", lua_sprite_destroy);
  lua_register(L, "sprite_set_position", lua_sprite_set_position);
  lua_register(L, "sprite_set_size", lua_sprite_set_size);
  lua_register(L, "sprite_set_frame_count", lua_sprite_set_frame_count);
  lua_register(L, "sprite_set_frame_time", lua_sprite_set_frame_time);
  lua_register(L, "sprite_update", lua_sprite_update);
  lua_register(L, "sprite_get_x", lua_sprite_get_x);
  lua_register(L, "sprite_get_y", lua_sprite_get_y);
  //end sprites

  //inventory
  lua_register(L, "inventory_get", lua_inventory_get_inventory);
  //end inventory
  printf("all functions registered!\n");

}


/**
Tilemap functions
*/

int lua_map_get_name(lua_State* L)
{
  if(lua_gettop(L) == 1)
  {
    tilemap_t* tilemap = (tilemap_t*)lua_touserdata(L, 1);
    if(tilemap != NULL)
    {
      lua_pushstring(L, tilemap->map_name);
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}

int lua_map_get_width(lua_State* L)
{
  if(lua_gettop(L) == 1)
  {
    tilemap_t* tilemap = (tilemap_t*)lua_touserdata(L, 1);
    if(tilemap != NULL)
    {
      lua_pushnumber(L, tilemap->width);
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}

int lua_map_get_height(lua_State* L)
{
  if(lua_gettop(L) == 1)
  {
    tilemap_t* tilemap = (tilemap_t*)lua_touserdata(L, 1);
    if(tilemap != NULL)
    {
      lua_pushnumber(L, tilemap->height);
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}

int lua_map_set_tile(lua_State* L)
{
  if(lua_gettop(L) == 4)
  {
    tilemap_t* tilemap = (tilemap_t*)lua_touserdata(L, 1);
    int x, y, id;
    x = lua_tonumber(L, 2);
    y = lua_tonumber(L, 3);
    id = lua_tonumber(L, 4);

    if(tilemap != NULL)
    {
      if(x >= 0 && x < tilemap->width * 32)
      {
        if(y >=0 && y < tilemap->height * 32)
        {
          int real_x, real_y;
          real_x = floor(x / 32);
          real_y = floor(y / 32);

          int index = real_x * tilemap->height + real_y;
          tilemap->tiles[index].id = id;
          return 0;
        }
      }
      return 0;
    }
    else
      return 0;
  }
  else
    return 0;
}

int lua_map_get_tile(lua_State* L) //gets the ID
{
  if(lua_gettop(L) == 3) //tilemap, x, y
  {
    tilemap_t* tilemap = (tilemap_t*)lua_touserdata(L, 1);
    int x, y;
    x = lua_tonumber(L, 2);
    y = lua_tonumber(L, 3);

    if(tilemap != NULL)
    {
      if(x >= 0 && x < tilemap->width * 32)
      {
        if(y >= 0 && y < tilemap->width * 32)
        {
          int tx, ty;
          tx = floor(x / 32);
          ty = floor(y / 32);

          int index = tx * tilemap->height + ty;
          lua_pushnumber(L, tilemap->tiles[index].id);
          return 1;
        }
      }
    }
  }
  return 0;
}

/**
End tilemap functions
*/
