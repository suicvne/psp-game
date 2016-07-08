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

  int button_interact; //the button to interact with objects in the overworld (npcs, signs, etc.)
  int button_use_item; //uses the current inventory item.

  #ifdef SDL_VERS
  int mouse_x; //used in the level editor.
  int mouse_y;

  int button_angle_increase;
  #endif

} input_t;

typedef enum INPUT_BUTTON_TYPES {

  INPUT_BUTTON_INTERACT = 0, //default (X) on PSP
  INPUT_BUTTON_USE_ITEM = 1, //default (square) on PSP

  INPUT_BUTTON_ANGLE_INCREASE = 6 //for SDL

} INPUT_BUTTON_TYPES;

extern input_t input_current_frame;
extern input_t input_last_frame;

//input_t* input_create();
void input_initialize();

#ifdef SDL_VERS
vector_t input_mouse_to_world(camera_t* camera); //TODO: change to new format
#endif

void input_update();
void input_begin_frame();

int input_is_button_just_pressed(INPUT_BUTTON_TYPES button_type);

#endif //___INPUT_H___
