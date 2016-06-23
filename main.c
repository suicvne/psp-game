#ifdef PSP
#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspctrl.h>
#include <oslib/oslib.h>
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

#define DEADZONE 0.5
#define LOOK_DEADZONE 0.5
#define PLAYER_SPEED 2
#define SPEED 4
#define KNOCKBACK_SPEED 8

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
int initSDL()
{
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
  kPlayer = player_create();
  kTestEntity = sprite_create("res/edgelord.png", SPRITE_TYPE_PNG);

  kTestEntity->rectangle.x = 100;
  kTestEntity->rectangle.y = 100;
  kTestEntity->rectangle.w = 32;
  kTestEntity->rectangle.h = 32;

  kBackgroundSprite = sprite_create("res/bg.png", SPRITE_TYPE_PNG);
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

int inside_viewport(const camera_t camera, const sprite_t* sprite)
{
  rectangle_t camera_viewport = camera_get_viewport(camera);
  return rectangle_intersects(&camera_viewport, &sprite->rectangle);
}

vector_t calculate_knockback(int movement_angle)
{
  //1. invert player movement angle
  int inverse_angle = 360 - movement_angle;
  vector_t movement;
  movement.x = KNOCKBACK_SPEED * cos(angleToRad(inverse_angle)); //angular movement
  movement.y = KNOCKBACK_SPEED * sin(angleToRad(inverse_angle));

  return movement;
}

tilemap_t* load_level(const char* dir, const char* filename)
{
  printf("loading file from '%s'..", filename);
  return tilemap_read_from_file(dir, filename);
}

int serializer_test()
{
  char* buffer = malloc(sizeof(char) * 16);
  int pointer = 0;
  serializer_write_string(buffer, &pointer, "map v2");
  return serializer_write_to_file(buffer, 16, "str_test.bin");
}

int main(void)
{
  SetupCallbacks();
  initSubsystem();
  initialize_globals();

  serializer_test();

  tilemap_t* tilemap_test = load_level("./map", "level.bin");
  printf("tilemap_test: %d x %d\n", tilemap_test->width, tilemap_test->height);

  int skip = 0;
  while(1)
  {
    tilemap_update(tilemap_test, kCamera);
    /*
    Set rotation
    */
    vector_t stickInput = get_analogue_movement();

    if(stickInput.x != 0.0f || stickInput.y != 0.0f)
    {
      if(vector_magnitude(stickInput) > DEADZONE) //LOOK_DEADZONE
      {
        int angular_value = (radToDegree(atan2(stickInput.y, stickInput.x)) - 90);
        player_setlookangle(kPlayer, angular_value);
      }
    }
    /*
    End Set rotation
    */

    /*
      Begin movement
    */

    if(stickInput.x != 0.0f || stickInput.y != 0.0f)
    {
      if(vector_magnitude(stickInput) < LOOK_DEADZONE) //DEADZONE
      {
        stickInput.x = 0;
        stickInput.y = 0;
      }

      float xtrajectory, ytrajectory;
      xtrajectory = -(PLAYER_SPEED * stickInput.x);
      ytrajectory = -(PLAYER_SPEED * stickInput.y);

      kCamera.x += xtrajectory;
      kCamera.y += ytrajectory;

      if(collides_with(kPlayer, kTestEntity)) //|| tilemap_is_player_colliding(tilemap_test, kPlayer, kCamera))
      {
        kCamera.x -= xtrajectory;
        kCamera.y -= ytrajectory;
      }
    }

    /*
      End movement
    */


    // draw last
    if(!skip)
    {
      #ifdef PSP
      oslStartDrawing();
      //oslSetAlpha(OSL_FX_DEFAULT, 255);
      //oslSetAlpha(OSL_FX_ALPHA | OSL_FX_COLOR, RGBA(48, 48, 48, 128));
      oslClearScreen(RGBA(0, 0, 0, 255));
      //sprite_draw_camera(kBackgroundSprite, kCamera);
      tilemap_draw(tilemap_test, kCamera);
      if(inside_viewport(kCamera, kTestEntity))
      {
        sprite_draw_camera(kTestEntity, kCamera);
      }
      sprite_draw(kPlayer->sprite); //TODO: seperate player draw?

      //draw_forest(tilemap_test->width * 32, tilemap_test->height * 32);
      oslDrawStringf(10, 2, "%s", tilemap_test->map_name);
      //oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 255));
      oslEndDrawing();
      #endif
    }
    #ifdef PSP
    oslEndFrame();
    skip = oslSyncFrame();
    #endif
  }

  //tilemap_destroy(tilemap_test);
  #ifdef PSP
  oslEndGfx();
  oslQuit();
  #endif

  return 0;
}
