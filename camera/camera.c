#include "camera.h"

camera_t* camera_create(int x, int y)
{
  camera_t* return_value = malloc(sizeof(camera_t));

  return_value->x = x;
  return_value->y = y;

  return return_value;
}

void camera_destroy(camera_t* camera)
{
  assert(camera != NULL);
  free(camera);
}

vector_t camera_player_to_world(const camera_t* camera)
{
  vector_t value = {
    -camera->x + ORIGIN_X - 16,
    -camera->y + ORIGIN_Y - 16
  };

  return value;
};

void camera_move(camera_t* camera, const vector_t movement_amount)
{
  camera->x += movement_amount.x;
  camera->y += movement_amount.y;
};
