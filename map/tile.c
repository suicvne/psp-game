#include "tile.h"

/**
GunnerWolf at the Game Dev League Discord channel helped me out with this! Thank you <3
*/
vector_t tile_get_location_by_id(short id)
{
  vector_t return_value;
  if(id > (SHEET_WIDTH * SHEET_HEIGHT) / (TILE_WIDTH * SHEET_HEIGHT))
  {
    return return_value; //should default to 0, 0
  }

  return_value.x = id * TILE_WIDTH;
  return_value.y = 0;
  while(return_value.x > SHEET_WIDTH)
  {
    return_value.x -= SHEET_WIDTH;
    return_value.y += TILE_HEIGHT;
  }

  return return_value;
}

short tile_get_id_by_location(int x, int y)
{
  if(x > SHEET_WIDTH || y > SHEET_WIDTH)
    return 0;

    return 0; //TODO
}
