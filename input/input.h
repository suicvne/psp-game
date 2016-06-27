/**
Mostly created for the sake of SDL
*/

#ifndef ___INPUT_H___
#define ___INPUT_H___

#include "../vector/vector.h"
#include "../globals.h"
#include <assert.h>

#if PSP
#include <pspctrl.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#endif

typedef struct input_t {

  vector_t analogue_input; //used to determine analogue movement/player movement

} input_t;

input_t* input_create();
void input_destroy(input_t* input);
void input_update(input_t* input);

#endif //___INPUT_H___
