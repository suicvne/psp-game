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

extern const float DEADZONE;
extern const int PLAYER_SPEED;
extern const int PLAYER_SPEED_EDITOR;
extern const int PLAYER_WIDTH;
extern const int PLAYER_HEIGHT;

typedef struct player_t {

  sprite_t* main_sprite;
  sprite_t* lift_sprite; //used for carrying something large over the player's head
  sprite_t* hold_sprite; //used for general item/weapon(?) holding.

  int use_timer; //timer used to hold the time the player has been 'using' an item

  int is_using; //true if using, false if not

} player_t;

player_t* player_create();
void player_destroy(player_t* player);
void player_setlookangle(player_t* player, int angle);
void player_update(player_t* player);
void player_update_animation_offset(player_t* player, const vector_t* movement);

#endif //___PLAYER_H___
