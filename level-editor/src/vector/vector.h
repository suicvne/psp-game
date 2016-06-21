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
  float output;
  #ifdef PSP
  asm("sqrt.s %0,%1" : "=f" (output) : "f" (c));
  #else
  output = sqrt(c);
  #endif
  return output;
}

float vector_magnitude(vector_t v);
vector_t vector_add(vector_t a, vector_t b);

#endif //___VECTOR_H___
