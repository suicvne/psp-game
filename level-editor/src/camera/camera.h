#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "../vector/vector.h"
#include "../graphics/common.h" //for rectangle_t
#include <stdlib.h> //abs

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define HALF_SCREEN_WIDTH 240
#define HALF_SCREEN_HEIGHT 136
#define ORIGIN_X 224
#define ORIGIN_Y 120

typedef struct {

  float x, y; //the X and Y offsets at which to draw everything else.

} camera_t;

static inline vector_t camera_player_to_world(const camera_t camera)
{
  vector_t value = {
    -camera.x + ORIGIN_X - 16,
    -camera.y + ORIGIN_Y - 16
  };

  return value;
};

static inline rectangle_t camera_player_to_world_rectangle(const camera_t camera)
{
  //they can set w/h later tbh
  rectangle_t value;
  value.x = -camera.x + ORIGIN_X - 16;
  value.y = -camera.y + ORIGIN_Y - 16;

  return value;
};

static inline rectangle_t camera_get_viewport(const camera_t camera)
{
  rectangle_t value;
  value.w = SCREEN_WIDTH;
  value.h = SCREEN_HEIGHT;
  value.x = -camera.x;
  value.y = -camera.y;

  return value;
};

static inline void camera_move(camera_t* camera, const vector_t movement_amount)
{
  camera->x += movement_amount.x;
  camera->y += movement_amount.y;
};

#endif //___CAMERA_H___
