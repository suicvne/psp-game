#include "camera.h"
#include "vector.h"

//#include "../player/player.h"

/*
int SCREEN_WIDTH = 480; //PSP default resolution
int SCREEN_HEIGHT = 272;
int HALF_SCREEN_WIDTH = 240;
int HALF_SCREEN_HEIGHT = 136;
*/


camera_t* camera_create(int x, int y, int screen_width, int screen_height)
{
  camera_t* return_value = malloc(sizeof(camera_t));

  return_value->x = x;
  return_value->y = y;

  return_value->screen_width = screen_width;
  return_value->screen_height = screen_height;
  return_value->half_width = floor(screen_width / 2);
  return_value->half_height = floor(screen_height / 2);

  return return_value;
}

void camera_destroy(camera_t* camera)
{
  assert(camera != NULL);
  free(camera);
}

/*
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
*/

void camera_move(camera_t* camera, const vector_t movement_amount)
{
  camera->x += movement_amount.x;
  camera->y += movement_amount.y;
}

/*
int camera_get_player_origin(int* x, int* y)
{
	(*x) = HALF_SCREEN_WIDTH - (PLAYER_WIDTH / 4);
	(*y) = HALF_SCREEN_HEIGHT - (PLAYER_HEIGHT / 4);
	
	return 0;
}
*/
