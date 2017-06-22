/**
Mostly created for the sake of SDL
*/

#ifndef ___INPUT_H___
#define ___INPUT_H___

#include <rocklevel/vector.h>
#include "../globals.h"
#include "../lib_extensions/camera.h"
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

  int button_hotbar_next; //advances the hotbar
  int button_hotbar_back;

  int button_dpad_up;
  int button_dpad_down;
  int button_dpad_left;
  int button_dpad_right;

  int button_inventory_open;

  #ifdef SDL_VERS
  int mouse_x; //used in the level editor.
  int mouse_y;

  int button_angle_increase;
  #endif

} input_t;

typedef enum INPUT_BUTTON_TYPES {

  INPUT_BUTTON_INTERACT = 0, //default (X) on PSP
  INPUT_BUTTON_USE_ITEM = 1, //default (square) on PSP
  INPUT_BUTTON_HOTBAR_NEXT = 2, //default (R) on PSP
  INPUT_BUTTON_HOTBAR_BACK = 3, //default (L) on PSP
  INPUT_BUTTON_DUP = 4,
  INPUT_BUTTON_DDOWN = 5,
  INPUT_BUTTON_DLEFT = 6,
  INPUT_BUTTON_DRIGHT = 7,

  INPUT_BUTTON_INVENTORY_OPEN = 8, //default (triangle) on PSP

  INPUT_BUTTON_ANGLE_INCREASE = 20 //for SDL

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
