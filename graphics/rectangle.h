#ifndef ___RECTANGLE___H___
#define ___RECTANGLE___H___

#if PSP
#include <oslib/oslib.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#endif

struct _camera_t;

typedef struct _rectangle_t {
  int w, h;
  int x, y;

  int left, right;
  int top, bottom;
} rectangle_t;

#if SDL_VERS
static inline SDL_Rect rectangle_to_sdl(const struct _rectangle_t* rect)
{
  SDL_Rect return_value;
  return_value.x = rect->x;
  return_value.y = rect->y;
  return_value.w = rect->w;
  return_value.h = rect->h;

  return return_value;
}
#endif

void rectangle_draw(const struct _rectangle_t* rect, uint32_t color);
void rectangle_draw_filled(const struct _rectangle_t* rect, uint32_t color);

void rectangle_draw_camera(const struct _rectangle_t* rect, uint32_t color, const struct _camera_t* camera);
void rectangle_draw_filled_camera(const struct _rectangle_t* rect, uint32_t color, const struct _camera_t* camera);

void rectangle_update_bounds(struct _rectangle_t* a);
int rectangle_intersects(struct _rectangle_t* a, struct _rectangle_t* b);

struct _rectangle_t camera_player_to_world_rectangle(const struct _camera_t* camera);
struct _rectangle_t camera_get_viewport(const struct _camera_t* camera);

#endif //___RECTANGLE___H___
