//pls

#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <stdlib.h>
#include <math.h>

typedef struct _vector_t {

  float x, y;

} vector_t;

static inline float fastsqrt(float c)
{
  #ifdef PSP
    float output;
    asm("sqrt.s %0,%1" : "=f" (output) : "f" (c));
    return output;
  #elif SDL_VERS
    return sqrt(c);
  #endif
}

float vector_magnitude(struct _vector_t v);
struct _vector_t vector_add(struct _vector_t a, struct _vector_t b);

#endif //___VECTOR_H___
