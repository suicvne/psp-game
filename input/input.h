/**
Mostly created for the sake of SDL
*/

#ifndef ___INPUT_H___
#define ___INPUT_H___

#include "../vector/vector.h"
#include "../globals.h"
#include "../camera/camera.h"
#include <assert.h>

#if PSP
#include <pspctrl.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#endif

typedef struct input_t {

  vector_t analogue_input; //used to determine analogue movement/player movement
  #ifdef SDL_VERS
  int mouse_x;
  int mouse_y;
  #endif

} input_t;

input_t* input_create();

#ifdef SDL_VERS
vector_t input_mouse_to_world(input_t* input, camera_t* camera);
#endif

void input_destroy(input_t* input);
void input_update(input_t* input);

#endif //___INPUT_H___
