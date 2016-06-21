#ifndef __SPRITE_H__
#define __SPRITE_H__

#ifdef PSP
#include <oslib/oslib.h>
#else
#include <SDL2/SDL.h>
#endif
#include <assert.h>
#include "../graphics/common.h"
#include "../camera/camera.h"

/**
Contains various properties for holding for sprites
*/
typedef struct
{
  //int x, y; //x and y positions in the world
  //int w, h; //w and height of a single frame of a sprite_t
  rectangle_t rectangle;
  int yframeoffset;
  int frames, currentframe, frametime; //total animation frames, current frame in animation, and the time in ticks between frames.
  int elapsedticks;
  #ifdef PSP
  OSL_IMAGE* image;
  #else
  SDL_Texture* image;
  int angle;
  #endif
} sprite_t;

typedef enum {

  SPRITE_TYPE_PNG = 0,
  SPRITE_TYPE_JPEG = 1

} SPRITE_TYPE;

sprite_t* sprite_create(const char* sprite_path, SPRITE_TYPE type);

void sprite_destroy(sprite_t* sprite);

void sprite_set_angle(sprite_t* sprite, int angle);

void sprite_draw(sprite_t* sprite);
void sprite_draw_offset(sprite_t* sprite, int x_offset, int y_offset);
void sprite_draw_camera(sprite_t* sprite, const camera_t camera);
void sprite_draw_camera_source(sprite_t* sprite, const camera_t camera, int x, int y, int sx, int sy, int w, int h);
void sprite_draw_camera_factor(sprite_t* sprite, const camera_t camera, float movement_factor);
void sprite_draw_camera_factor_offset(sprite_t* sprite, const camera_t camera, float movement_factor, int x_offset, int y_offset);
void sprite_draw_camera_pointer_factor_offset(sprite_t* sprite, const camera_t camera, int x, int y, float movement_factor, int x_offset, int y_offset);
void sprite_update(sprite_t* sprite);

#endif //__SPRITE_H__
