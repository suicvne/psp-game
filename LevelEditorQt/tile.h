#ifndef TILE_H
#define TILE_H

#include "vector.h"

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
#define SHEET_WIDTH 256
#define SHEET_HEIGHT 256

class tile
{
public:
    tile();
    typedef enum {
      TILE_TYPE_PASSABLE = 0,
      TILE_TYPE_SOLID = 1
    } TILE_TYPE;

    vector get_location_by_id();
    short get_id_by_location(int x, int y);

    void set_id(short _id);
    void set_angle(short _angle);
    void set_tile_type(TILE_TYPE _type);

private:
    short id;
    short angle;
    TILE_TYPE tile_type;
};

#endif // TILE_H
