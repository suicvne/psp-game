#include "tilemap.h"

#include "../player/player.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <rocklevel/serialization_reader.h>

void tilemap_update(tilemap_t* map, const camera_t* cam)
{
  //TODO: regular updating.

  /*
  Lua
  */

  if(map->lua_state != NULL && !kLevelEditorMode)
  {
    lua_getglobal(map->lua_state, "onUpdate");
    if(lua_isnil(map->lua_state, -1))
    {/*Function dne*/}
    else if(lua_pcall(map->lua_state, 0, 0, 0) != 0)
    {
      char buffer[64];
      sprintf(buffer, "Fatal Error during onUpdate: %s", lua_tostring(map->lua_state, -1));
      report_fatal_error(buffer);
      lua_pop(map->lua_state, 1);
    }
  }

  /*
  End Lua
  */
}

void tilemap_draw(tilemap_t* map, const camera_t* cam)
{
  //TODO: move player drawing into here for proper layering
  /*
  Get bounds for drawing
  */
  int min_x, max_x, min_y, max_y, x_iter, y_iter;

  camera_get_index_bounds(cam, map, &min_x, &max_x, &min_y, &max_y);

  for(x_iter = zeroize(min_x); x_iter < min(max_x, map->width); x_iter++)
  {
    for(y_iter = zeroize(min_y); y_iter < min(max_y, map->height); y_iter++)
    {
      int index = x_iter * map->height + y_iter;

      if(index < 0 || index > (map->height * map->width))
      {
        continue;
      } //skip NULL tiles
      else
      {
        tile_t tile = map->tiles[index];
        vector_t sheet_location = tile_get_location_by_id(tile.id);
        sprite_set_angle(map->tileset, tile.angle);
        if(tile.angle > 0)
        {
          int cx, cy;
          switch(tile.angle) //...i'm..not proud of this..
          {
            case 90:
            cx = 0; cy = 32;
            break;
            case 180:
            cx = 32; cy = 32;
            break;
            case 270:
            cx = 32; cy = 0;
            break;
            default:
            cx = 0; cy = 0;
            break;
          }
          sprite_set_center_point(map->tileset, cx, cy);
        }
        sprite_draw_camera_source(map->tileset, *cam, floor(x_iter * 32), floor(y_iter * 32), sheet_location.x, sheet_location.y, 32, 32);
        sprite_set_angle(map->tileset, 0);
        sprite_set_center_point(map->tileset, 0, 0);
      }
    }
  }

  //oslSetAlpha(OSL_FX_ALPHA, RGBA(255, 127, 0, 255));
  //oslSetAlpha(OSL_FX_ALPHA, 255);
  //oslDrawFillRect(0, 0, 480, 272, RGBA(255, 255, 255, 255));

  //oslDrawFillRect(20, 20, 100, 100, RGBA(255, 127, 0, 255));

  //oslSetAlpha(OSL_FX_ALPHA, 255);



  /*
  Lua
  */
  if(map->lua_state != NULL && !kLevelEditorMode)
  {
    lua_getglobal(map->lua_state, "onDraw");
    if(lua_isnil(map->lua_state, -1))
    {/*Function dne*/}
    else if(lua_pcall(map->lua_state, 0, 0, 0) != 0)
    {
      char buffer[256];
      sprintf(buffer, "Fatal Error during onDraw: %s", lua_tostring(map->lua_state, -1));
      report_fatal_error(buffer);
      lua_pop(map->lua_state, 1);
    }
  }
  /*
  End Lua
  */
}

int tilemap_load_lua_file(lua_State* L, const char* filename)
{
  if(kLevelEditorMode)
    return 0;

  int error;

  if(serializer_get_file_size(filename) > 0)
  {
    error = luaL_loadfile(L, filename);
    return error;
  }
  else
    return 1;
}

int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t* camera)
{
  rectangle_t player_hitbox = camera_player_to_world_rectangle(camera);
    player_hitbox.w = 32; player_hitbox.h = 32;

  rectangle_t theoretical_level_bounds;
  theoretical_level_bounds.x = 32;
  theoretical_level_bounds.y = 32;
  theoretical_level_bounds.w = (map->width) * 32;
  theoretical_level_bounds.h = (map->height) * 32;

  if(rectangle_intersects(&player_hitbox, &theoretical_level_bounds))
    return 0;
  else
    return 1;
}

void tilemap_report_lua_errors(lua_State* L, int status)
{
  if(status != 0)
  {
    char buffer[256];
    sprintf(buffer, "fatal error in Lua script: %s", lua_tostring(L, -1));
    report_fatal_error(buffer);
    lua_pop(L, 1); //remove error from Lua stack
  }
}

void camera_get_index_bounds(const camera_t* camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y)
{
  int half_map_width, half_map_height;
  half_map_width = ((tilemap->width * 32) / 2);
  half_map_height = ((tilemap->height * 32) / 2);


  float corrected_x, corrected_y;
  corrected_x = -camera->x;
  corrected_y = -camera->y;

  (*min_x) = floor(
    (corrected_x) / 32
  );
  (*max_x) = ceil(
    (corrected_x + SCREEN_WIDTH) / 32
  );
  (*min_y) = floor(
    (corrected_y) / 32
  );
  (*max_y) = ceil(
    (corrected_y + SCREEN_HEIGHT) / 32
  );
}
