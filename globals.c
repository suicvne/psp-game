#include "globals.h"
#include "sprites/sprite.h"
#include "player/player.h"
#include "camera/camera.h"

int kQuit;
struct player_t* kPlayer = NULL;
struct sprite_t* kTestEntity = NULL; //22x29 sprite on a 32x32 sheet
struct sprite_t* kBackgroundSprite = NULL;
struct sprite_t* kForest = NULL;
struct camera_t* kCamera;

#ifdef PSP
OSL_FONT* kMainFont;
#endif

#ifdef SDL_VERS
SDL_Window* kSdlWindow;
SDL_Renderer* kSdlRenderer;
SDL_Event kSdlEvent;
#endif
