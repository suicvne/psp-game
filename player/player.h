
#ifndef ___PLAYER_H___
#define ___PLAYER_H___

struct _sprite_t;
struct _tilemap_t;
struct _vector_t;
struct _player_t;

extern const float DEADZONE;
extern const int PLAYER_SPEED;
extern const int PLAYER_SPEED_EDITOR;
extern const int PLAYER_WIDTH;
extern const int PLAYER_HEIGHT;

typedef struct _player_t {

  struct _sprite_t* main_sprite;
  sprite_t* lift_sprite; //used for carrying something large over the player's head
  sprite_t* hold_sprite; //used for general item/weapon(?) holding.

  int use_timer; //timer used to hold the time the player has been 'using' an item

  int is_using; //true if using, false if not

  int is_moving;
  float move_timer;

  float speed_x;
  float speed_y;

} player_t;

player_t* player_create();
void player_destroy(struct _player_t* player);
void player_setlookangle(struct _player_t* player, int angle);
void player_update(struct _player_t* player, tilemap_t* tilemap);

void player_update_animation_offset_by_vector(struct _player_t* player, const vector_t* movement);
void player_update_animation_offset_by_direction(struct _player_t* player, int direction); //0: left, 1: right, 2: up, 3: down

#endif //___PLAYER_H___
