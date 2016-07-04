#include "player.h"

void player_destroy(player_t* player)
{
  assert(player != NULL);
  //sprite_destroy(player->sprite);
  free(player);
}

void player_update_animation_offset(player_t* player, const vector_t* movement)
{
  int offset = 0;

  if(movement->x > 0.3f) //right
  {
    offset = 4;
  }
  else if(movement->x < -0.3f) //left
  {
    offset = 3;
  }

  if(movement->y > 0.3f) //down
  {
    offset = 0;
  }
  else if(movement->y < -0.3f) //up
  {
    offset = 5;
  }

  if(movement->x > 0.3f && movement->y > 0.3f) //right down
  {
    offset = 2;
  }
  else if(movement->x > 0.3f && movement->y < -0.3f) //right up
  {
    offset = 7;
  }
  else if(movement->x < -0.3f && movement->y > 0.3f) //left down
  {
    offset = 1;
  }
  else if(movement->x < -0.3f && movement->y < -0.3f) //left up
  {
    offset = 6;
  }

  player->main_sprite->yframeoffset = offset;
}

void player_update(player_t* player)
{
  vector_t stickInput = input_current_frame.analogue_input;
  if(stickInput.x != 0.0f || stickInput.y != 0.0f)
  {
    float magnitude = vector_magnitude(stickInput);

    if(magnitude > DEADZONE)
    {
      player_update_animation_offset(player, &stickInput);
    }

    if(magnitude < DEADZONE)
    {
      stickInput.x = 0;
      stickInput.y = 0;
    }

    float xtrajectory, ytrajectory;
    if(kLevelEditorMode)
    {
      xtrajectory = -(PLAYER_SPEED * stickInput.x);
      ytrajectory = -(PLAYER_SPEED * stickInput.y);
    }
    else
    {
      xtrajectory = -(PLAYER_SPEED * stickInput.x);
      ytrajectory = -(PLAYER_SPEED * stickInput.y);
    }

    kCamera->x += xtrajectory;
    kCamera->y += ytrajectory;

    if(xtrajectory != 0.0f || ytrajectory != 0.0f)
    { sprite_update(player->main_sprite); }
  }
  else
    player->main_sprite->currentframe = 0;
}

player_t* player_create()
{
  //TODO: redef originx and originy
  int orgX = ((480 / 2) - (PLAYER_WIDTH / 2));;
  int orgY = ((272 / 2) - (PLAYER_HEIGHT / 2));; //stop
  //int ORIGIN_X = ((480 / 2) - (PLAYER_WIDTH / 2));
  //int ORIGIN_Y = ((272 / 2) - (PLAYER_HEIGHT / 2));

  player_t* player = malloc(sizeof(player_t));
  player->main_sprite = sprite_create("res/ness.png", SPRITE_TYPE_PNG);
  player->main_sprite->rectangle.x = orgX; //center of the screen
  player->main_sprite->rectangle.y = orgY;
  player->main_sprite->rectangle.w = PLAYER_WIDTH;
  player->main_sprite->rectangle.h = PLAYER_HEIGHT;

  player->main_sprite->frametime = PLAYER_SPEED * 2.5;
  player->main_sprite->frames = 2; //no animation, yet

  player->lift_sprite = NULL; //TODO
  player->hold_sprite = NULL; //TODO

  #ifdef PSP
  oslSetImageTileSize(player->main_sprite->image, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
  #endif

  return player;
}

void player_setlookangle(player_t* player, int angle)
{
  sprite_set_angle(player->main_sprite, angle);
}
