//pls

#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <math.h>

typedef struct {

  float x, y;

} vector_t;

static inline float vector_magnitude(vector_t v)
{
  return sqrt((v.x * v.x) + (v.y * v.y));
};

static inline vector_t vector_add(vector_t a, vector_t b)
{
  vector_t value = {
    a.x + b.x,
    a.y + b.y
  };
  return value;
};

#endif //___VECTOR_H___
