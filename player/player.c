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

  player->sprite->yframeoffset = offset;
}

void player_update(player_t* player)
{
  vector_t stickInput = kInput->analogue_input;
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
    xtrajectory = -(PLAYER_SPEED * stickInput.x);
    ytrajectory = -(PLAYER_SPEED * stickInput.y);

    kCamera->x += xtrajectory;
    kCamera->y += ytrajectory;

    if(xtrajectory != 0.0f || ytrajectory != 0.0f)
    { sprite_update(player->sprite); }
  }
  else
    player->sprite->currentframe = 0;
}

player_t* player_create()
{
  //TODO: redef originx and originy
  int orgX = ((480 / 2) - (PLAYER_WIDTH / 2));;
  int orgY = ((272 / 2) - (PLAYER_HEIGHT / 2));; //stop
  //int ORIGIN_X = ((480 / 2) - (PLAYER_WIDTH / 2));
  //int ORIGIN_Y = ((272 / 2) - (PLAYER_HEIGHT / 2));

  player_t* player = malloc(sizeof(player_t));
  player->sprite = sprite_create("res/ness.png", SPRITE_TYPE_PNG);
  player->sprite->rectangle.x = orgX; //center of the screen
  player->sprite->rectangle.y = orgY;
  player->sprite->rectangle.w = PLAYER_WIDTH;
  player->sprite->rectangle.h = PLAYER_HEIGHT;

  player->sprite->frametime = PLAYER_SPEED * 2.5;
  player->sprite->frames = 2; //no animation, yet

  #ifdef PSP
  oslSetImageTileSize(player->sprite->image, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
  #endif

  //#ifdef PSP
  //player->sprite->image->centerX = 16;
  //player->sprite->image->centerY = 16; //for angling
  //#endif
  //oslSetImageTileSize(sprite, 0, 0, 32, 32);

  return player;
}

void player_setlookangle(player_t* player, int angle)
{
  sprite_set_angle(player->sprite, angle);
}
