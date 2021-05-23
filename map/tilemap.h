/**

Level Format Abridged Changelog

v1 - initial format: stores header, map_name, width, height, id of each tile in the map
v2 - appends the tileset filename after map_name

*/

#ifndef ___TILEMAP_H___
#define ___TILEMAP_H___

#define HEADER_0 'M'
#define HEADER_1 'S'
#define VERSION 05

struct _tile_t;
struct _sprite_t;
struct _camera_t;
struct lua_State;


typedef struct _tilemap_t {

  //Tiles will be stored in a single dimension array.
  //You can access objects by doing
  //tiles[x * h + y]
  struct _tile_t* tiles;
  int width, height;
  char* map_name;
  short surrounding_tile_id;
  char* tileset_path;
  char* foreground_tileset_path;
  struct _sprite_t* tileset;
	struct _sprite_t* foreground_tileset; //the tileset for layer 2 aka foreground.
  struct lua_State* lua_state;

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

int tilemap_load_lua_file(struct lua_State* L, const char* filename);
struct _tilemap_t* tilemap_create(int width, int height, int allocate_texture);
void tilemap_destroy(struct _tilemap_t* map);

void tilemap_update(struct _tilemap_t* map, const struct _camera_t* cam); //passing in the camera so I know what tiles I need to draw/update

/**
player is a global that is drawn inside of the tilemap_draw function for proper layering.
*/
void tilemap_draw(struct _tilemap_t* map, const struct _camera_t* cam);

void camera_get_index_bounds(const struct _camera_t* camera, 
            struct _tilemap_t* tilemap, 
            int* min_x, 
            int* max_x, 
            int* min_y, 
            int* max_y);

//int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t* camera);

void tilemap_report_lua_errors(struct lua_State* L, int status);

int tilemap_write_to_file(const char* filename, struct _tilemap_t* map);
int tilemap_verify_header(char* header, short version);
struct _tilemap_t* tilemap_read_from_file(const char* directory, const char* filename);

struct _tile_t tilemap_get_tile_at(struct _tilemap_t* tilemap, int x, int y);

#endif //___TILEMAP_H___
