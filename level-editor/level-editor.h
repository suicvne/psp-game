/**

Contains editor related functions.

This file is meant to consolodate all level editor exclusive features together for ease of use.

This should ONLY be used with the SDL Version of the game.

*/

#if SDL_VERS

#ifndef ___LEVEL_EDITOR_H___
#define ___LEVEL_EDITOR_H___

#include "../vector/vector.h"
#include "../globals.h"
#include "../player/player.h"
#include "../camera/camera.h"
#include "../map/tilemap.h"
#include "../input/input.h"
#include "ui/tinyfiledialogs.h"

typedef enum {

  EDITOR_SCREEN_EDIT = 0,
  EDITOR_SCREEN_PICK_TILE = 1

} EDITOR_SCREEN;

typedef struct level_editor_t {

  tilemap_t* tilemap;

  char* directory; //dir the level is to save to
  char* filename; //filename the level is to save as

  EDITOR_SCREEN current_screen;
  short current_tile_id;
  short current_tile_angle;

} level_editor_t;

extern const char* editor_dialog_filters[1];

level_editor_t* editor_create();
void editor_destroy(level_editor_t* editor);

int editor_load_level(level_editor_t* editor);
int editor_save_level(level_editor_t* editor);

void editor_extract_directory_from_path(const char* full_path, char* out_directory, char* out_filename);

void editor_handle_input(level_editor_t* editor);
void editor_draw(level_editor_t* editor);
void editor_update(level_editor_t* editor);

//DO NOT CALL THESE EXTERNALLY
void editor_draw_editor(level_editor_t* editor);
void editor_draw_pick_tile(level_editor_t* editor);

#endif //___LEVEL_EDITOR_H___

#endif //SDL_VERs
