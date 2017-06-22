#ifndef __NEW_CAMERA_H__
#define __NEW_CAMERA_H__

#include <rocklevel/camera.h>
#include <rocklevel/vector.h>


vector_t camera_player_to_world(const camera_t* camera);
int camera_get_player_origin(int* x, int* y);

#endif //__NEW_CAMERA_H__
