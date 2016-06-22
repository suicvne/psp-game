/**

defines various global variables to be reused in the lifetime of the game.

*/

#ifndef ___GLOBALS_H___
#define ___GLOBALS_H___

#include "sprites/sprite.h"
#include "player/player.h"
#include "camera/camera.h"

static player_t* kPlayer = NULL;
static sprite_t* kTestEntity = NULL; //22x29 sprite on a 32x32 sheet
static sprite_t* kBackgroundSprite = NULL;
static sprite_t* kForest = NULL;
static camera_t kCamera;

#ifdef PSP
static OSL_FONT* kMainFont;
#endif

#ifdef SDL_VERS
#include <SDL2/SDL.h> //TODO: this


#endif

#endif //___GLOBALS_H___
