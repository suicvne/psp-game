#include "globals.h"
#include "sprites/sprite.h"
#include "player/player.h"
#include "camera/camera.h"
#include "input/input.h"

#ifdef PSP
#include <oslib/oslib.h>
#endif

int kQuit;
int kLevelEditorMode;
int kUpdate = 1;
int kSkip = 0;

struct _player_t* kPlayer = NULL;
//struct sprite_t* kTestEntity = NULL; //22x29 sprite on a 32x32 sheet
struct _sprite_t* kBackgroundSprite = NULL;
struct _sprite_t* kForest = NULL;
struct _camera_t* kCamera = NULL;
struct _sprite_t* kItems = NULL; //contains the sheet for the items
struct _sprite_t* kInventory = NULL; //contains the inventory sprites
struct _sprite_t* kButtons = NULL;
//struct input_t* kInput = NULL;

#ifdef PSP
struct OSL_FONT* kMainFont;
#endif

#ifdef SDL_VERS
SDL_Window* kSdlWindow;
SDL_Renderer* kSdlRenderer;
SDL_Event kSdlEvent;
TTF_Font* kSdlFont;
float kDelta; //time delta
#endif
