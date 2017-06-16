#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___TILE_H___
#define ___TILE_H___

#include "vector.h"

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
  short angle;
  TILE_TYPE tile_type;
} tile_t;

extern vector_t tile_get_location_by_id(short id);
extern short tile_get_id_by_location(int x, int y);

#endif //___TILE_H___

#ifdef __cplusplus
}
#endif
