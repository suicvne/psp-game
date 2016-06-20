#ifndef ___PLAYER_H___
#define ___PLAYER_H___

#include "../sprites/sprite.h"
#include <malloc.h>
#include <assert.h>

//player is 32x32.
#define ORIGIN_X 224
#define ORIGIN_Y 120

typedef struct {

  sprite_t* sprite;

} player_t;

player_t* player_create();
void player_destroy(player_t* player);
void player_setlookangle(player_t* player, int angle);

#endif //___PLAYER_H___
