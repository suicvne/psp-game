#ifdef PSP
#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspctrl.h>
#include <oslib/oslib.h>
#endif

#if SDL_VERS
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "system/system_info.h"
#endif

#include "graphics/rectangle.h"
#include "graphics/text.h"

#include "sound/sound.h"

#include "message-box/message_box.h"

#include "callback.h" //for PSP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "player/player.h"
#include "sprites/sprite.h"
#include "vector/vector.h"
#include "camera/camera.h"
#include "globals.h"
#include "input/input.h"

#include "serialization/serialization_reader.h"
#include "serialization/serializer.h"

#include "map/tilemap.h"


#if PSP
PSP_MODULE_INFO("Mini Rust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12 * 1024);

/**
Initializes OSLib so i can't start doing stuff with my life
*/
int init_OSLib()
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

#if SDL_VERS

int init_window()
{
  char* windowTitle = "alpha";
  kSdlWindow = SDL_CreateWindow(windowTitle,
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );
  if(kSdlWindow == NULL)
  {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }
  return 0;
}

int init_renderer()
{
  kSdlRenderer = SDL_CreateRenderer(kSdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(kSdlRenderer == NULL)
  {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(kSdlWindow);
    SDL_Quit();
    return 1;
  }
  SDL_RenderSetLogicalSize(kSdlRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  return 0;
}

int shutdown_SDL()
{
  if(kSdlRenderer != NULL)
    SDL_DestroyRenderer(kSdlRenderer);

  if(kSdlWindow != NULL)
    SDL_DestroyWindow(kSdlWindow);

  TTF_Quit();
  IMG_Quit();
  Mix_CloseAudio();
  SDL_Quit();

  return 0;
}

int init_SDL()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  init_window();

  init_renderer();

  if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    fprintf(stderr, "Error initializing SDL_Image: %s\n", IMG_GetError());
    return 1;
  }

  if(TTF_Init() != 0)
  {
    fprintf(stderr, "Error initializing SDL_TTTF: %s\n", TTF_GetError());
    return 1;
  }

  if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) != 0)
  {
    fprintf(stderr, "Error initializing SDL_Mixer: %s\n", Mix_GetError());
    return 1;
  }

  return 0;
}
#endif

int init_subsystem()
{
  #if PSP
  return init_OSLib();
  #elif SDL_VERS
  return init_SDL();
  #endif
}

void initialize_globals(void)
{
  sound_initialize(); //not a global but all of my other initialization is done here sooooo

  kCamera = camera_create(0, 0);
  kPlayer = player_create();
  kForest = sprite_create("res/forest.png", SPRITE_TYPE_PNG);
  kInventory = sprite_create("res/inventory.png", SPRITE_TYPE_PNG);
  kItems = sprite_create("res/items.png", SPRITE_TYPE_PNG);

#ifdef PSP
  //kMainFont = oslLoadFontFile("flash0:/font/ltn0.pgf"); //ltn0
  kMainFont = oslLoadFontFile("res/hack.pgf"); //can't find the font in ppsspp on linux?
    oslIntraFontSetStyle(kMainFont, .4f, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 255), INTRAFONT_ALIGN_LEFT);
    oslSetFont(kMainFont);
#elif SDL_VERS
  kSdlFont = TTF_OpenFont("res/hack.ttf", 10);
  if(kSdlFont == NULL)
  {
    char buffer[256];
    sprintf(buffer, "Could not open font for reading: %s", TTF_GetError());
    report_fatal_error(buffer);
  }
#endif
}

void destroy_globals(void)
{
  camera_destroy(kCamera);
  player_destroy(kPlayer);
  sprite_destroy(kForest);
  sound_destroy();
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
  printf("loading file from '%s/%s.bin'..\n", dir, filename);
  return tilemap_read_from_file(dir, filename);
}

void update(tilemap_t* tilemap)
{
  input_update();

  if(kUpdate)
  {
    tilemap_update(tilemap, kCamera);
    player_update(kPlayer, tilemap);

    /*if(tilemap_is_player_colliding(tilemap, kPlayer, kCamera))
    {
      printf("Collision!\n");
    }*/
  }

  if(message_box_is_visible)
    message_box_update(); //always update message boxes to maintain consitency

  inventory_update();
}

void draw(tilemap_t* tilemap)
{
  #if PSP //step 1: clear screen
  if(!kSkip)
  {
    oslStartDrawing();
    oslClearScreen(RGBA(0, 0, 0, 255));
  #elif SDL_VERS
  SDL_RenderClear(kSdlRenderer);
  #endif

  //Do drawing in here

  tilemap_draw(tilemap, kCamera);
  sprite_draw(kPlayer->main_sprite);
  text_render_text(tilemap->map_name, 10, 2);
  if(message_box_is_visible)
    message_box_draw();

  //inventory_draw();
  //End do drawing in here

  #if PSP //present
  }
  oslEndDrawing();
  kSkip = oslSyncFrame();
  #elif SDL_VERS
  SDL_RenderPresent(kSdlRenderer);
  #endif
}

void output_system_info()
{
#ifdef SDL_VERS
  system_info_t info = get_system_info();
  printf("\n\n---SYSTEM INFO---\n\n");
  printf("Platform: %s\n", info.platform);
  printf("Subsystem: %s\n", info.subsystem);
  printf("CPU Logical Cores: %d\n", info.cpu_core_count);
  #ifdef SDL_VERS
  printf("SDL Version: %d.%d.%d\n", info.sdl_version_major, info.sdl_version_minor, info.sdl_version_patch);
  #endif
  printf("\n\n---END SYSTEM INFO---\n\n");
#endif
}

/**
 * @brief SDL_main
 * main
 * @param argc arg count
 * @param argv args
 * @return nothing
 */

int main(int argc, char** argv)
{
    printf("\n\nARGS TEST\n\n");
    int i;
    for(i = 0; i <  argc; i++)
    {
        printf("arg %d: %s\n", i, argv[i]);
        if(strcmp("--level-editor", argv[i]) == 0)
        {
            report_fatal_error("level editor mode no longer supported.");
            break;
        }
    }

#if PSP
    psp_setup_callbacks();
#endif
    init_subsystem();
    initialize_globals();

    output_system_info(); //output system info to console for convenience sake

    kQuit = 0;
    tilemap_t* tilemap_test = load_level("./res/maps", "level");
    printf("tilemap_test: %d x %d\n", tilemap_test->width, tilemap_test->height);

    #ifdef SDL_VERS
    float frame_time = 0.f;
    int previous_time = 0;
    int current_time = 0;
    //float deltaTime = 0.f;
    #endif

    while(!kQuit) //game loop
    {
#ifdef SDL_VERS
      previous_time = current_time;
      current_time = SDL_GetTicks();
      kDelta = (current_time - previous_time) / 1000.0f;
      frame_time += kDelta;

      if(frame_time >= 0.01f)
      {
        frame_time = 0;
        update(tilemap_test);
      }
#else
	update(tilemap_test);
#endif

      draw(tilemap_test);
    }

    tilemap_destroy(tilemap_test);
    destroy_globals();

#if PSP
    oslEndGfx();
    oslQuit();
#elif SDL_VERS
    shutdown_SDL();
#endif

    /*
  printf("\n---Args---\n\n");
  int i;
  for(i = 0; i < argc; i++)
  {
    printf("%d: %s\n", i, argv[i]);
    if(strcmp("--level-editor", argv[i]) == 0)
    {
      kLevelEditorMode = 1;
      printf("\n!!! LEVEL EDITOR MODE !!!\n\tLua scripts will not be executed.");
      break;
    }
  }
  printf("\n---End args---\n\n");

  #if PSP
  if(kLevelEditorMode)
  {
    report_fatal_error("How the fuck did you get level editor mode on the PSP?");
  }
  #endif

  psp_setup_callbacks();
  init_subsystem();
  initialize_globals();

  kQuit = 0;

  if(kLevelEditorMode)
  {
    #if SDL_VERS
    level_editor_t* editor = editor_create();
    while(!kQuit)
    {
      editor_update(editor);

      editor_draw(editor);
    }
    #endif
  }
  else
  {
    tilemap_t* tilemap_test = load_level("./map", "level.bin");
    printf("tilemap_test: %d x %d\n", tilemap_test->width, tilemap_test->height);

    while(!kQuit)
    {
      update(tilemap_test);

      draw(tilemap_test);
    }

    tilemap_destroy(tilemap_test);
  }
  destroy_globals();

  #if PSP
  oslEndGfx();
  oslQuit();
  #elif SDL_VERS
  shutdown_SDL();
  #endif

  return 0;
  */
    return 0;
}
