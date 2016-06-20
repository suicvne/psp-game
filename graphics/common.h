/**

defines some common functions for graphics work like rectangles and stuff

*/

#ifndef ___GRAPHICS_COMMON_H___
#define ___GRAPHICS_COMMON_H___

#include <malloc.h>

typedef struct {
  int w, h;
  int x, y;

  int left, right;
  int top, bottom;
} rectangle_t;

static inline void rectangle_update_bounds(rectangle_t* a)
{
  a->left = a->x;
  a->right = a->x + a->w;

  a->top = a->y;
  a->bottom = a->y + a->h;
}

static inline int rectangle_intersects(rectangle_t* a, rectangle_t* b)
{
  rectangle_update_bounds(a);
  rectangle_update_bounds(b);

  if(
    ((a->left < b->right) & (a->right > b->left))
    &&
    ((a->top < b->bottom) & (a->bottom > b->top))
  )
    return 1;

  return 0;
};

#endif //___GRAPHICS_COMMON_H___
