#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "../vector/vector.h"
#include <stdlib.h> //abs
#include <assert.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int HALF_SCREEN_WIDTH;
extern const int HALF_SCREEN_HEIGHT;

extern int ORIGIN_X;
extern int ORIGIN_Y;

typedef struct camera_t {

  float x, y; //the X and Y offsets at which to draw everything else.

} camera_t;

camera_t* camera_create(int x, int y);
void camera_destroy(camera_t* camera);

vector_t camera_player_to_world(const camera_t* camera);

void camera_move(camera_t* camera, const vector_t movement_amount);

int camera_get_player_origin(int* x, int* y);

#endif //___CAMERA_H___
