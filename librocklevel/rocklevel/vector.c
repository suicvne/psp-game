#include "vector.h"

float vector_magnitude(vector_t v)
{
  return sqrt((v.x * v.x) + (v.y * v.y));
}

vector_t vector_add(vector_t a, vector_t b)
{
  vector_t value = {
    a.x + b.x,
    a.y + b.y
  };
  return value;
}
