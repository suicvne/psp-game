#ifndef ___TILEMAP_H___
#define ___TILEMAP_H___

#include "tile.h"
#include "../camera/camera.h"
#include "../sprites/sprite.h"
#include "../player/player.h"
#include "../graphics/common.h"
#include "../serialization/serializer.h"
#include "../serialization/serialization_reader.h"
#include <malloc.h>
#include <assert.h>

#define HEADER_0 'M'
#define HEADER_1 'S'
#define VERSION 01

typedef struct {

  //Tiles will be stored in a single dimension array.
  //You can access objects by doing
  //tiles[x * h + y]
  tile_t* tiles;
  int width, height;
  char* map_name;
  short surrounding_tile_id;

} tilemap_t;

static inline int zeroize(int input)
{
  if(input < 0)
    return 0;
  else
    return input;
}

static inline int min(int a, int b)
{
  if(a > b)
    return b;
  else
    return a;
}

tilemap_t* tilemap_create(int width, int height);
void tilemap_destroy(tilemap_t* map);

void tilemap_update(tilemap_t* map, const camera_t cam); //passing in the camera so I know what tiles I need to draw/update
void tilemap_draw(tilemap_t* map, const camera_t cam, sprite_t* tileset);

void camera_get_index_bounds(const camera_t camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y);

int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t camera);

int tilemap_write_to_file(const char* filename, tilemap_t* map);
int tilemap_verify_header(char* header, short version);
tilemap_t* tilemap_read_from_file(const char* filename);
#endif //___TILEMAP_H___
