/**

Defines common functions for the map's Lua interpreter.

*/

//TODO: use Lua's boolean type when available

#ifndef ___MAP_LUA_FUNCTIONS_H___
#define ___MAP_LUA_FUNCTIONS_H___

#ifdef PSP
#include <oslib/oslib.h>
#endif

struct lua_State;

struct _tilemap_t;

extern struct lua_State* current_lua_state;

int lua_resume_execution(struct lua_State* L);

/**
Drawing functions
*/
int lua_map_print(struct lua_State* L);
int lua_map_draw_text(struct lua_State* L);
int lua_map_rgba(struct lua_State* L);
void lua_map_register_functions(struct lua_State* L, struct _tilemap_t* tilemap);
int lua_map_draw_rect(struct lua_State* L);
int lua_map_draw_rect_camera(struct lua_State* L);

int lua_map_set_blending(struct lua_State* L);
int lua_map_test_blending(struct lua_State* L);

int lua_map_draw_sprite(struct lua_State* L); //map will have its own draw sprite function to differentiate between with or without camera
/**
End Drawing functions
*/

/**
Tilemap functions - map_lua_functions.c
*/

int lua_map_get_name(struct lua_State* L);
int lua_map_get_width(struct lua_State* L);
int lua_map_get_height(struct lua_State* L);

int lua_map_set_tile(struct lua_State* L);
int lua_map_set_tile_layer2(struct lua_State* L);
int lua_map_get_tile(struct lua_State* L); //gets the ID

/**
End Tilemap functions
*/

/**
Player functions - map_lua_functions_player.c
*/

//Retrieve the player's X transformed with Camera coordinates
int lua_player_get_x(struct lua_State* L);

//Retrieve the player's Y transformed with Camera coordinates
int lua_player_get_y(struct lua_State* L);

//Retrieve the player's current direction based on its y-offset
int lua_player_get_direction(struct lua_State* L);

/**
End Player Functions
*/

/**
Input Functions - map_lua_functions_input.c
*/

int lua_input_is_button_pressed(struct lua_State* L);

/**
End Input Functions
*/

/**
Message Box Functions - map_lua_functions_message_box.c
*/

int lua_message_box_is_visible(struct lua_State* L);
int lua_message_box_set_visible(struct lua_State* L);
int lua_message_box_set_message(struct lua_State* L);
int lua_message_box_get_message(struct lua_State* L);
int lua_message_box_update(struct lua_State* L);
int lua_message_box_draw(struct lua_State* L);

/**
End Message Box Functions
*/

/**
Sprite functions - map_lua_functions_sprite.c
*/

int lua_sprite_create(struct lua_State* L);

int lua_sprite_set_position(struct lua_State* L);
int lua_sprite_set_size(struct lua_State* L);
int lua_sprite_set_frame_count(struct lua_State* L);
int lua_sprite_set_frame_time(struct lua_State* L);

int lua_sprite_get_x(struct lua_State* L);
int lua_sprite_get_y(struct lua_State* L);

int lua_sprite_update(struct lua_State* L); //This is under sprites because updating is independent from drawing

int lua_sprite_destroy(struct lua_State* L);

/**
End sprite functions
*/

/**
Inventory functions - map_lua_functions_inventory.c
*/

int lua_inventory_get_inventory(struct lua_State* L); //returns inventory as table
//int lua_inventory_push_item(lua_State* L);
//int lua_inventory_pop_item(lua_State* L);
//int lua_inventory_get_item(lua_State* L);

/**
End inventory functions
*/

#endif //___MAP_LUA_FUNCTIONS_H___
