/**

Level Format Abridged Changelog

v1 - initial format: stores header, map_name, width, height, id of each tile in the map
v2 - appends the tileset filename after map_name

*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___TILEMAP_H___
#define ___TILEMAP_H___

#include "tile.h"

#define HEADER_0 'M'
#define HEADER_1 'S'
#define VERSION 05

typedef struct tilemap_t {

  //Tiles will be stored in a single dimension array.
  //You can access objects by doing
  //tiles[x * h + y]
  tile_t* tiles;
  int width, height;
  char* map_name;
  short surrounding_tile_id;
  char* tileset_path;
  char* foreground_tileset_path;

} tilemap_t;

/*
static inline int zeroize(int input)
{
  if(input < 0)
    return 0;
  else
    return input;
}
*/

/*
int min(int a, int b)
{
  if(a > b)
    return b;
  else
    return a;
}
*/

extern tilemap_t* tilemap_create(int width, int height, int allocate_texture);
extern void tilemap_destroy(tilemap_t* map);

//void tilemap_update(tilemap_t* map); //passing in the camera so I know what tiles I need to draw/update
//void tilemap_draw(tilemap_t* map);

extern int tilemap_write_to_file(const char* filename, tilemap_t* map);
extern int tilemap_verify_header(char* header, short version);

extern tilemap_t* tilemap_read_from_file(const char* filename);

#endif //___TILEMAP_H___

#ifdef __cplusplus
}
#endif
