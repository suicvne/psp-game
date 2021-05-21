#ifndef ___TILE_H___
#define ___TILE_H___

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define SHEET_WIDTH 256
#define SHEET_HEIGHT 256

struct _sprite_t;
struct _vector_t;

typedef enum {
  TILE_TYPE_PASSABLE = 0,
  TILE_TYPE_SOLID = 1
} TILE_TYPE;

typedef struct _tile_t {
  short id;
  short id_layer2; //the ID of the secondary block (layer 2)
  short angle;
  TILE_TYPE tile_type;
} tile_t;

struct _vector_t tile_get_location_by_id(short id);

/**
the same as `tile_get_location_by_id` except it uses the width and height of the supplied sprite instead of the default values
*/
struct _vector_t tile_get_location_by_id_given_sheet(short id, struct _sprite_t* sprite);
short tile_get_id_by_location(int x, int y);

#endif //___TILE_H___
