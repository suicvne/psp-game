#include <pspkernel.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspctrl.h>
#include <oslib/oslib.h>

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
  return 0;
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
  kMainFont = oslLoadFontFile("res/ltn0.pgf"); //can't find the font in ppsspp on linux?
    oslIntraFontSetStyle(kMainFont, .4f, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 255), INTRAFONT_ALIGN_LEFT);
    oslSetFont(kMainFont);
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

int draw_forest(int width, int height)
{
  int FOREST_SIZE = 513;
  //oslSetAlpha(OSL_FX_ALPHA, 200);
  oslSetAlpha(OSL_FX_ALPHA, 200);
  //void sprite_draw_camera_factor_offset(sprite_t* sprite, const camera_t camera, float movement_factor, int x_offset, int y_offset)
  int x, y;

  for(x = -FOREST_SIZE; x < width + (FOREST_SIZE * 2); x += FOREST_SIZE)
  {
    for(y = -FOREST_SIZE; y < height + (FOREST_SIZE * 2); y += FOREST_SIZE)
    {
      sprite_draw_camera_pointer_factor_offset(kForest, kCamera, x, y, 2.0f, 0, 0);
    }
  }

  //void sprite_draw_camera_pointer_factor_offset(sprite_t* sprite, const camera_t camera, int x, int y, float movement_factor, int x_offset, int y_offset);

  //sprite_draw_camera_factor_offset(kForest, kCamera, 1.5f, forest_offset_x, 0);
  oslSetAlpha(OSL_FX_DEFAULT, 255);
}

tilemap_t* load_level(const char* filename)
{
  printf("loading file from '%s'..", filename);
  return tilemap_read_from_file(filename);
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
  initOSLib();
  initialize_globals();

  serializer_test();

  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

  tilemap_t* tilemap_test = load_level("level.bin");
  //tilemap_t* tilemap_test = tilemap_create(32, 32);
  //tilemap_test->map_name = "Test Map v2 Format";
  printf("tilemap_test: %d x %d\n", tilemap_test->width, tilemap_test->height);
  //tilemap_write_to_file("level.bin", tilemap_test);

  int skip = 0;
  while(1)
  {
    SceCtrlData pad;
    sceCtrlPeekBufferPositive(&pad, 1);

    /*
    Set rotation
    */
    float padZeroX, padZeroY;
    padZeroX = ((pad.Lx) - 128) / 127.0f; //128 is directly in middle so i normalize it here to be closer to say unity (-1 to +1)
    padZeroY = ((pad.Ly) - 128) / 127.0f;

    vector_t stickInput;
    stickInput.x = padZeroX;
    stickInput.y = padZeroY;

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

      draw_forest(tilemap_test->width * 32, tilemap_test->height * 32);
      oslDrawStringf(10, 2, "%s", tilemap_test->map_name);
      //oslDrawFillRect(0, 0, 480, 272, RGBA(0, 0, 0, 255));
      oslEndDrawing();
    }
    oslEndFrame();
    skip = oslSyncFrame();
  }

  //tilemap_destroy(tilemap_test);
  oslEndGfx();
  oslQuit();

  return 0;
}
