#ifdef PSP
#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspctrl.h>
#include <oslib/oslib.h>
#elif SDL_VERS
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include "callback.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "sprites/sprite.h"
#include "vector/vector.h"
#include "camera/camera.h"
#include "globals.h"

#include "serialization/serialization_reader.h"
#include "serialization/serializer.h"

#include "map/tilemap.h"

#define SPEED 4
#define KNOCKBACK_SPEED 8

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 480
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_HEIGHT 272
#endif

#ifdef PSP
PSP_MODULE_INFO("Analogue Sample", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12 * 1024);

/**
Initializes OSLib so i can't start doing stuff with my life
*/
int initOSLib()
{
  oslInit(0);
  oslInitGfx(OSL_PF_8888, 1);
  oslInitAudio();
  oslSetQuitOnLoadFailure(1);
  oslSetKeyAutorepeatInit(40);
  oslSetKeyAutorepeatInterval(10);
  oslIntraFontInit(INTRAFONT_CACHE_MED);
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
  return 0;
}
#endif

vector_t get_analogue_movement()
{
  #ifdef PSP
  SceCtrlData pad;
  sceCtrlPeekBufferPositive(&pad, 1);

  float padZeroX, padZeroY;
  padZeroX = ((pad.Lx) - 128) / 127.0f; //128 is directly in middle so i normalize it here to be closer to say unity (-1 to +1)
  padZeroY = ((pad.Ly) - 128) / 127.0f;

  vector_t return_value = {
    padZeroX,
    padZeroY
  };
  return return_value;
  #elif SDL_VERS
  vector_t return_value = {0.0f, 0.0f};
  return return_value;
  #endif
}

#ifdef SDL_VERS

int initWindow()
{
  kSdlWindow = SDL_CreateWindow("Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if(kSdlWindow == NULL)
  {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
}

int initRenderer()
{
  kSdlRenderer = SDL_CreateRenderer(kSdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(kSdlRenderer == NULL)
  {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(kSdlWindow);
    SDL_Quit();
    return 1;
  }
}

int shutdown_SDL()
{
  if(kSdlRenderer != NULL)
    SDL_DestroyRenderer(kSdlRenderer);

  if(kSdlWindow != NULL)
    SDL_DestroyWindow(kSdlWindow);

  return 0;
}

int initSDL()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  initWindow();

  initRenderer();

  if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)
  {
    fprintf(stderr, "Error initializing SDL_Image: %s\n", SDL_GetError());
    return 1;
  }

  return 0;
}
#endif

int initSubsystem()
{
  #ifdef PSP
  return initOSLib();
  #elif SDL_VERS
  return initSDL();
  #endif
}

void initialize_globals(void)
{
  kCamera = malloc(sizeof(camera_t));

  kPlayer = player_create();

  kForest = sprite_create("res/forest.png", SPRITE_TYPE_PNG);
  //kCamera initialized, not a pointer.
  //kMainFont = oslLoadFontFile("flash0:/font/ltn0.pgf"); //ltn0
  #ifdef PSP
  kMainFont = oslLoadFontFile("res/ltn0.pgf"); //can't find the font in ppsspp on linux?
    oslIntraFontSetStyle(kMainFont, .4f, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 255), INTRAFONT_ALIGN_LEFT);
    oslSetFont(kMainFont);
  #endif
}

int collides_with(const player_t* player, const sprite_t* sprite)
{
  //1. translate player
  rectangle_t player_hitbox = camera_player_to_world_rectangle(kCamera);
    player_hitbox.w = 32; player_hitbox.h = 32;
  return rectangle_intersects(&player_hitbox, &sprite->rectangle);
}

tilemap_t* load_level(const char* dir, const char* filename)
{
  printf("loading file from '%s'..", filename);
  return tilemap_read_from_file(dir, filename);
}

int update(tilemap_t* tilemap)
{
  #ifdef SDL_VERS
  while(SDL_PollEvent(&kSdlEvent))
  {
    if(kSdlEvent.type == SDL_QUIT)
    {
      kQuit = 1;
      shutdown_SDL();
    }
  }
  #endif

  tilemap_update(tilemap, kCamera);
  
  player_update(kPlayer);
}

int draw(tilemap_t* tilemap)
{
  #ifdef PSP
  // draw last
  //if(!skip)
  //{
    oslStartDrawing();
    oslClearScreen(RGBA(0, 0, 0, 255));
    tilemap_draw(tilemap, kCamera);

    sprite_draw(kPlayer->sprite); //TODO: seperate player draw?

    //draw_forest(tilemap_test->width * 32, tilemap_test->height * 32);
    oslDrawStringf(10, 2, "%s", tilemap->map_name);
    oslEndDrawing();
  //}
  oslEndFrame();
  //skip = oslSyncFrame();
  oslSyncFrame();
  #elif SDL_VERS
  SDL_RenderClear(kSdlRenderer);

  tilemap_draw(tilemap, kCamera);

  SDL_RenderPresent(kSdlRenderer);
  #endif
}

int main(void)
{
  SetupCallbacks();
  initSubsystem();
  initialize_globals();

  tilemap_t* tilemap_test = load_level("./map", "level.bin");
  printf("tilemap_test: %d x %d\n", tilemap_test->width, tilemap_test->height);

  kQuit = 0;
  while(!kQuit)
  {
    update(tilemap_test);

    draw(tilemap_test);
  }

  //tilemap_destroy(tilemap_test);
  #ifdef PSP
  oslEndGfx();
  oslQuit();
  #elif SDL_VERS
  shutdown_SDL();
  #endif

  return 0;
}
