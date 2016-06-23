#include "player.h"

void player_destroy(player_t* player)
{
  assert(player != NULL);
  //sprite_destroy(player->sprite);
  free(player);
}

player_t* player_create()
{
  player_t* player = malloc(sizeof(player_t));
  player->sprite = sprite_create("res/edgelord.png", SPRITE_TYPE_PNG);
  player->sprite->rectangle.x = ORIGIN_X;
  player->sprite->rectangle.y = ORIGIN_Y;
  player->sprite->rectangle.w = 32;
  player->sprite->rectangle.h = 32;


  player->sprite->frames = 0; //no animation, yet
  #ifdef PSP
  player->sprite->image->centerX = 16;
  player->sprite->image->centerY = 16; //for angling
  #endif
  //oslSetImageTileSize(sprite, 0, 0, 32, 32);

  return player;
}

void player_setlookangle(player_t* player, int angle)
{
  sprite_set_angle(player->sprite, angle);
}
