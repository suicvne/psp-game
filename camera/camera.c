#include "camera.h"

#include "../player/player.h"

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 272;
const int HALF_SCREEN_WIDTH = 240;
const int HALF_SCREEN_HEIGHT = 136;


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
  //TODO: make this constant somehow
  int ORIGIN_X = HALF_SCREEN_WIDTH - (PLAYER_WIDTH / 4);
  int ORIGIN_Y = HALF_SCREEN_HEIGHT - (PLAYER_HEIGHT / 4);

  vector_t value = {
    -camera->x + ORIGIN_X,
    -camera->y + ORIGIN_Y
  };

  return value;
};

void camera_move(camera_t* camera, const vector_t movement_amount)
{
  camera->x += movement_amount.x;
  camera->y += movement_amount.y;
};

int camera_get_player_origin(int* x, int* y)
{
	(*x) = HALF_SCREEN_WIDTH - (PLAYER_WIDTH / 4);
	(*y) = HALF_SCREEN_HEIGHT - (PLAYER_HEIGHT / 4);
	
	return 0;
}