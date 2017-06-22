/**

Level Format Abridged Changelog

v1 - initial format: stores header, map_name, width, height, id of each tile in the map
v2 - appends the tileset filename after map_name

*/

#ifndef ___TILEMAP_H___
#define ___TILEMAP_H___

#include "tile.h"
#include "../graphics/rectangle.h"
#include "../globals.h"
#include "../common.h"
#include "../camera/camera.h"
#include "../sprites/sprite.h"
//#include "../player/player.h"
#include "../serialization/serializer.h"
#include "../serialization/serialization_reader.h"
#include "lua/map_lua_functions.h"

#include <stdlib.h>
//#include <malloc.h>
#include <assert.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define HEADER_0 'M'
#define HEADER_1 'S'
#define VERSION 04

typedef struct tilemap_t {

  //Tiles will be stored in a single dimension array.
  //You can access objects by doing
  //tiles[x * h + y]
  tile_t* tiles;
  int width, height;
  char* map_name;
  short surrounding_tile_id;
  char* tileset_path;
  sprite_t* tileset;
  lua_State* lua_state;

} tilemap_t;

static inline int zeroize(int input)
{
  if(input < 0)
    return 0;
  else
    return input;
}

/*
int min(int a, int b)
{
  if(a > b)
    return b;
  else
    return a;
}
*/

int tilemap_load_lua_file(lua_State* L, const char* filename);
tilemap_t* tilemap_create(int width, int height, int allocate_texture);
void tilemap_destroy(tilemap_t* map);

void tilemap_update(tilemap_t* map, const camera_t* cam); //passing in the camera so I know what tiles I need to draw/update
void tilemap_draw(tilemap_t* map, const camera_t* cam);

void camera_get_index_bounds(const camera_t* camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y);

//int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t* camera);

void tilemap_report_lua_errors(lua_State* L, int status);

int tilemap_write_to_file(const char* filename, tilemap_t* map);
int tilemap_verify_header(char* header, short version);
tilemap_t* tilemap_read_from_file(const char* directory, const char* filename);

tile_t tilemap_get_tile_at(tilemap_t* tilemap, int x, int y);

#endif //___TILEMAP_H___
