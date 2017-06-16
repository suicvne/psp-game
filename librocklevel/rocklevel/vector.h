//pls
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___VECTOR_H___
#define ___VECTOR_H___

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {

  float x, y;

} vector_t;

extern float vector_magnitude(vector_t v);
extern vector_t vector_add(vector_t a, vector_t b);

#endif //___VECTOR_H___

#ifdef __cplusplus
}
#endif
