#ifndef ___RECTANGLE___H___
#define ___RECTANGLE___H___

#include "common.h"
#include "../camera/camera.h"

#if PSP
#include <oslib/oslib.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#endif

typedef struct rectangle_t {
  int w, h;
  int x, y;

  int left, right;
  int top, bottom;
} rectangle_t;

void rectangle_draw(const rectangle_t* rect, uint32_t color);
void rectangle_draw_filled(const rectangle_t* rect, uint32_t color);

void rectangle_draw_camera(const rectangle_t* rect, uint32_t color, const camera_t* camera);
void rectangle_draw_filled_camera(const rectangle_t* rect, uint32_t color, const camera_t* camera);

void rectangle_update_bounds(rectangle_t* a);
int rectangle_intersects(rectangle_t* a, rectangle_t* b);

rectangle_t camera_player_to_world_rectangle(const camera_t* camera);
rectangle_t camera_get_viewport(const camera_t* camera);

#endif //___RECTANGLE___H___