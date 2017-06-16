#include "tile.h"

tile::tile()
{
    this->angle = 0;
    this->id = 0;
    this->tile_type = tile::TILE_TYPE_PASSABLE;
}

vector tile::get_location_by_id()
{
    vector return_value;
    int max_tiles = (SHEET_WIDTH * SHEET_HEIGHT) / (TILE_WIDTH * TILE_HEIGHT);
    if(this->id > max_tiles)
    {
      return return_value; //should default to 0, 0
    }

    return_value.x = this->id * TILE_WIDTH;
    return_value.y = 0;
    while(return_value.x >= SHEET_WIDTH)
    {
      return_value.x -= SHEET_WIDTH;
      return_value.y += TILE_HEIGHT;
    }
    return return_value;
}

short tile::get_id_by_location(int x, int y)
{
    if(x > SHEET_WIDTH || y > SHEET_WIDTH)
        return 0;
    return (x + (y * 8)); //TODO why is this TODO??
}

void tile::set_id(short _id)
{
    this->id = _id;
}

void tile::set_angle(short _angle)
{
    this->angle = _angle;
}

void tile::set_tile_type(TILE_TYPE _type)
{
    this->tile_type = _type;
}
