#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <SDL2/SDL.h>

struct _rectangle_t;
struct _vector_t;
struct _camera_t;
struct SDL_Texture;
enum _SPRITE_TYPE;

/**
Contains various properties for holding for sprites

OSL_IMAGE defines many of the pieces kept seperate for SDL. Angle and center point being two of them. Hence, it is recommended
to use standard functions for setting angle and center point as opposed to manually modifying OSL_IMAGE.
*/
typedef struct _sprite_t
{
  //int x, y; //x and y positions in the world
  //int w, h; //w and height of a single frame of a struct _sprite_t

  // THIS WAS RECENTLY CHANGED TO A POINTER AS OF 5-20-2021
  struct _rectangle_t* rectangle;
  int yframeoffset;
  int frames, currentframe, frametime; //total animation frames, current frame in animation, and the time in ticks between frames.
  int elapsedticks;

  #ifdef PSP
  OSL_IMAGE* image;
  #else
  struct SDL_Texture* image;
  int angle;
  SDL_Point center; //defines the centre point to rotate at
  #endif
} sprite_t;

typedef enum _SPRITE_TYPE {

  SPRITE_TYPE_PNG = 0,
  SPRITE_TYPE_JPEG = 1

} SPRITE_TYPE;

/**
Creates a sprite pointer using the appropriate loading method given its appropriate SPRITE_TYPE.

As of right now, only PNG textures are supported using the SDL backend.
Rectangle parameters are also completely zero-ed out.
*/
struct _sprite_t* sprite_create(const char* sprite_path, enum _SPRITE_TYPE type);

void sprite_destroy(struct _sprite_t* sprite);

void sprite_set_angle(struct _sprite_t* sprite, int angle);
void sprite_set_center_point(struct _sprite_t* sprite, int cx, int cy);

void sprite_draw(struct _sprite_t* sprite);
void sprite_draw_source(struct _sprite_t* sprite, int x, int y, int sx, int sy, int w, int h);
void sprite_draw_offset(struct _sprite_t* sprite, int x_offset, int y_offset);
void sprite_draw_camera(struct _sprite_t* sprite, const struct _camera_t camera);
void sprite_draw_camera_source(struct _sprite_t* sprite, const struct _camera_t camera, int x, int y, int sx, int sy, int w, int h);
void sprite_draw_camera_factor(struct _sprite_t* sprite, const struct _camera_t camera, float movement_factor);
void sprite_draw_camera_factor_offset(struct _sprite_t* sprite, const struct _camera_t camera, float movement_factor, int x_offset, int y_offset);
void sprite_draw_camera_pointer_factor_offset(struct _sprite_t* sprite, const struct _camera_t camera, int x, int y, float movement_factor, int x_offset, int y_offset);
void sprite_update(struct _sprite_t* sprite);

struct _vector_t sprite_get_location_by_index(struct _sprite_t* sprite, int sprite_size, int index);

#endif //__SPRITE_H__
