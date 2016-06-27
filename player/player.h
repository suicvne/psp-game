#ifndef ___PLAYER_H___
#define ___PLAYER_H___

#include "../sprites/sprite.h"
#include "../input/input.h"
#include "../vector/vector.h"
#include "../globals.h"
#include <malloc.h>
#include <assert.h>

#ifdef PSP
#include <pspctrl.h>
#endif

static const float DEADZONE = 0.5f;
static const int PLAYER_SPEED = 2;
static const int PLAYER_SPEED_EDITOR = 4;
static const int PLAYER_WIDTH = 15;
static const int PLAYER_HEIGHT = 24;

typedef struct player_t {

  sprite_t* sprite;

} player_t;

player_t* player_create();
void player_destroy(player_t* player);
void player_setlookangle(player_t* player, int angle);
void player_update(player_t* player);
void player_update_animation_offset(player_t* player, const vector_t* movement);

#endif //___PLAYER_H___
