#ifndef __NEW_TILEMAP_H__
#define __NEW_TILEMAP_H__

#include <rocklevel/tilemap.h>
#include <rocklevel/camera.h>
#include "../player/player.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void tilemap_update(tilemap_t* map, const camera_t* cam);
void tilemap_draw(tilemap_t* map, const camera_t* cam);

int tilemap_load_lua_file(lua_State* L, const char* filename);
int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t* camera);
void tilemap_report_lua_errors(lua_State* L, int status);

//why is this in tilemap again?
void camera_get_index_bounds(const camera_t* camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y);

#endif //__NEW_TILEMAP_H__
