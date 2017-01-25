//pls

#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {

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

float vector_magnitude(vector_t v);
vector_t vector_add(vector_t a, vector_t b);

#endif //___VECTOR_H___
