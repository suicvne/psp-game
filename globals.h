/**

defines various global variables to be reused in the lifetime of the game.

*/

#ifndef ___GLOBALS_H___
#define ___GLOBALS_H___

extern int kQuit;
extern struct player_t* kPlayer;
extern struct sprite_t* kTestEntity; //22x29 sprite on a 32x32 sheet
extern struct sprite_t* kForest;
extern struct camera_t* kCamera;

#ifdef PSP
extern OSL_FONT* kMainFont;
#endif

#ifdef SDL_VERS
#include <SDL2/SDL.h> //TODO: this
extern SDL_Window* kSdlWindow;
extern SDL_Renderer* kSdlRenderer;
extern SDL_Event kSdlEvent;
#endif

#endif //___GLOBALS_H___
