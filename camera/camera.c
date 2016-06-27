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

rectangle_t camera_player_to_world_rectangle(const camera_t* camera)
{
  //they can set w/h later tbh
  rectangle_t value;
  value.x = -camera->x + ORIGIN_X - 16;
  value.y = -camera->y + ORIGIN_Y - 16;

  return value;
};

rectangle_t camera_get_viewport(const camera_t* camera)
{
  rectangle_t value;
  value.w = SCREEN_WIDTH;
  value.h = SCREEN_HEIGHT;
  value.x = -camera->x;
  value.y = -camera->y;

  return value;
};

void camera_move(camera_t* camera, const vector_t movement_amount)
{
  camera->x += movement_amount.x;
  camera->y += movement_amount.y;
};
