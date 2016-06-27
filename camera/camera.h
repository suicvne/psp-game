#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "../vector/vector.h"
#include "../graphics/common.h" //for rectangle_t
#include <stdlib.h> //abs
#include <assert.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define HALF_SCREEN_WIDTH 240
#define HALF_SCREEN_HEIGHT 136
#define ORIGIN_X 224
#define ORIGIN_Y 120

typedef struct camera_t {

  float x, y; //the X and Y offsets at which to draw everything else.

} camera_t;

camera_t* camera_create(int x, int y);
void camera_destroy(camera_t* camera);

vector_t camera_player_to_world(const camera_t* camera);

rectangle_t camera_player_to_world_rectangle(const camera_t* camera);

rectangle_t camera_get_viewport(const camera_t* camera);

void camera_move(camera_t* camera, const vector_t movement_amount);

#endif //___CAMERA_H___
