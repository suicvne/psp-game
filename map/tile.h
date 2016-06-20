#ifndef ___TILE_H___
#define ___TILE_H___

#include "../vector/vector.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define SHEET_WIDTH 256
#define SHEET_HEIGHT 256

typedef enum {
  TILE_TYPE_PASSABLE = 0,
  TILE_TYPE_SOLID = 1
} TILE_TYPE;

typedef struct {
  short id;
  //vector_t sheet_location;
  TILE_TYPE tile_type;
} tile_t;

vector_t tile_get_location_by_id(short id);

#endif //___TILE_H___
