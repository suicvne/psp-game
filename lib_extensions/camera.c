#include "camera.h"
#include "../player/player.h"

vector_t camera_player_to_world(const camera_t* camera)
{
  int originX = (kScreenWidth / 2) - (PLAYER_WIDTH / 4);
  int originY = (kScreenHeight / 2) - (PLAYER_HEIGHT / 4);

  vector_t value = {
    -camera->x + originX,
    -camera->y + originY
  };
  return value;
}

int camera_get_player_origin(int* x, int* y)
{
  (*x) = (kScreenWidth / 2) - (PLAYER_WIDTH / 4);
  (*y) = (kScreenHeight / 2) - (PLAYER_HEIGHT / 4);

  return 0; //for now
}
