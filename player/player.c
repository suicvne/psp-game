#include "player.h"

const float DEADZONE = 0.5f;
const int PLAYER_SPEED = 2;
const int PLAYER_SPEED_EDITOR = 4;
const int PLAYER_WIDTH = 15;
const int PLAYER_HEIGHT = 24;


void player_destroy(player_t* player)
{
  assert(player != NULL);
  //sprite_destroy(player->sprite);
  free(player);
}

//0: left, 1: right, 2: up, 3: down
void player_update_animation_offset_by_direction(player_t* player, int direction)
{
  int offset;
  switch (direction)
  {
    case 0:
      offset = 3;
      break;
    case 1:
      offset = 4;
      break;
    case 2:
      offset = 5;
      break;
    case 3:
      offset = 0;
      break;
  }

  player->main_sprite->yframeoffset = offset;
}

void player_update_animation_offset_by_vector(player_t* player, const vector_t* movement)
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

void player_update(player_t* player, tilemap_t* tilemap)
{
  float move_speed = 1;
  vector_t stickInput = input_current_frame.analogue_input;

  if(player->is_moving == 0)
  {
    player->main_sprite->currentframe = 0;
    vector_t player_current_position = camera_player_to_world(kCamera);

    int ptx, pty;
    if(stickInput.x > 0.0f) //right
    {
      ptx = floor(player_current_position.x / TILE_WIDTH);
      pty = floor(player_current_position.y / TILE_HEIGHT);
      tile_t next_tile = tilemap_get_tile_at(tilemap, ptx + 1, pty);
      if(next_tile.tile_type == TILE_TYPE_PASSABLE)
      {
        player->is_moving = 1;
        player->move_timer = 32;
        player->speed_x = -move_speed;
        player->speed_y = 0;
      }

      player_update_animation_offset_by_direction(player, 1);
    }
    else if(stickInput.x < 0.0f) //left
    {
      ptx = floor(player_current_position.x / TILE_WIDTH);
      pty = floor(player_current_position.y / TILE_WIDTH);
      tile_t next_tile = tilemap_get_tile_at(tilemap, ptx - 1, pty);
      if(next_tile.tile_type == TILE_TYPE_PASSABLE)
      {
        player->is_moving = 1;
        player->move_timer = 32;
        player->speed_x = move_speed;
        player->speed_y = 0;
      }

      player_update_animation_offset_by_direction(player, 0);
    }
    else if(stickInput.y > 0.0f) //down
    {
      ptx = floor(player_current_position.x / TILE_WIDTH);
      pty = floor(player_current_position.y / TILE_HEIGHT);
      tile_t next_tile = tilemap_get_tile_at(tilemap, ptx, pty + 1);
      if(next_tile.tile_type == TILE_TYPE_PASSABLE)
      {
        player->is_moving = 1;
        player->move_timer = 32;
        player->speed_x = 0;
        player->speed_y = -move_speed;
      }

      player_update_animation_offset_by_direction(player, 3);
    }
    else if(stickInput.y < 0.0f) //up
    {
      ptx = floor(player_current_position.x / TILE_WIDTH);
      pty = floor(player_current_position.y / TILE_HEIGHT);
      tile_t next_tile = tilemap_get_tile_at(tilemap, ptx, pty - 1);
      if(next_tile.tile_type == TILE_TYPE_PASSABLE)
      {
        player->is_moving = 1;
        player->move_timer = 32;
        player->speed_x = 0;
        player->speed_y = move_speed;
      }

      player_update_animation_offset_by_direction(player, 2);
    }
  }

  if(player->is_moving == 1)
  {
    sprite_update(player->main_sprite);

    kCamera->x += player->speed_x;
    kCamera->y += player->speed_y;

    player->move_timer -= move_speed;
    if(player->move_timer <= 0)
      player->is_moving = 0;
  }

  #ifdef OLD_MOVEMENT
  vector_t stickInput = input_current_frame.analogue_input;
  if(stickInput.x != 0.0f || stickInput.y != 0.0f)
  {
    float magnitude = vector_magnitude(stickInput);

    if(magnitude > DEADZONE)
    {
      player_update_animation_offset(player, &stickInput);
    }
    else if(magnitude < DEADZONE)
    {
      stickInput.x = 0;
      stickInput.y = 0;
    }

    float xtrajectory, ytrajectory;

    #if SDL
    if(kLevelEditorMode)
    {
    #endif
    //lol
      xtrajectory = -(PLAYER_SPEED * stickInput.x);
      ytrajectory = -(PLAYER_SPEED * stickInput.y);
    #if SDL
    }
    else
    {
      xtrajectory = -(PLAYER_SPEED * stickInput.x);
      ytrajectory = -(PLAYER_SPEED * stickInput.y);
    }
    #endif

    kCamera->x += xtrajectory;
    kCamera->y += ytrajectory;

    if(xtrajectory != 0.0f || ytrajectory != 0.0f)
    { sprite_update(player->main_sprite); }

  }
  else
    player->main_sprite->currentframe = 0;
  #endif
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

  player->lift_sprite = NULL; //TODO: these
  player->hold_sprite = NULL; //TODO: these

  player->move_timer = 0;
  player->is_moving = 0;

  player->speed_x = 0;
  player->speed_y = 0;

  #ifdef PSP
  oslSetImageTileSize(player->main_sprite->image, 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
  #endif

  inventory_create(); //to create the inventory and stuff

  return player;
}

void player_setlookangle(player_t* player, int angle)
{
  sprite_set_angle(player->main_sprite, angle);
}
