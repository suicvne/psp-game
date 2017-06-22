#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "vector.h"
#include <stdlib.h> //abs
#include <assert.h>


typedef struct camera_t {

  float x, y; //the X and Y offsets at which to draw everything else.
  int screen_width, screen_height;
  int half_width, half_height;

} camera_t;

camera_t* camera_create(int x, int y, int screen_width, int screen_height);
void camera_destroy(camera_t* camera);

//vector_t camera_player_to_world(const camera_t* camera);

void camera_move(camera_t* camera, const vector_t movement_amount);

//int camera_get_player_origin(int* x, int* y);

#endif //___CAMERA_H___

#ifdef __cplusplus
}
#endif
