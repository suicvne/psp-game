#if SDL_VERS

#include "level-editor.h"

SDL_Scancode CONTROL_SCANCODE = SDL_SCANCODE_LCTRL;

const char* editor_dialog_filters[1] = { "*.bin" };
/**
Notes on tinyfd

lTheSaveFileName = tinyfd_saveFileDialog(
		"let us save this password",
		"passwordFile.txt",
		2,
		lFilterPatterns,
		NULL);

	if (!lTheSaveFileName) return 1 ;

	lIn = fopen(lTheSaveFileName, "w");
	if (!lIn)
	{
		tinyfd_messageBox(
			"Error",
			"Can not open this file in write mode",
			"ok",
			"error",
			1);
		return 1 ;
	}
	fputs(lThePassword, lIn);
	fclose(lIn);

	lTheOpenFileName = tinyfd_openFileDialog(
		"let us read the password back",
		"",
		2,
		lFilterPatterns,
		NULL,
		0);
*/

level_editor_t* editor_create()
{
  level_editor_t* editor = malloc(sizeof(level_editor_t));

  editor->tilemap = tilemap_create(32, 32, 1 /*true*/ ); //32 x 32 map by default.

  editor->directory = NULL;
  editor->filename = NULL;

  editor->current_screen = EDITOR_SCREEN_EDIT;
  editor->current_tile_id = 0;
  editor->current_tile_angle = 0;

  /**
  For retrieving graphical mode info
  */
  tinyfd_inputBox("tinyfd_query", NULL, NULL);

  printf("using tinyfiledialogs\nCopyright (c) 2014 - 2016 Guillaume Vareille http://ysengrin.com http://tinyfiledialogs.sourceforge.net");
  printf("\n\ngraphics mode: %s\n", tinyfd_response);

  //tinyfd_messageBox("Editor Initiated", "Initiated blank 32x32 level.", "ok", "information", 0);

  #if __APPLE__
  CONTROL_SCANCODE = SDL_SCANCODE_LGUI; //command key on Apple computers, winkey on windows
  #endif

  return editor;
}

void editor_destroy(level_editor_t* editor)
{
  tilemap_destroy(editor->tilemap);
  free(editor);
}

int editor_load_level(level_editor_t* editor)
{
  const char* full_filename = tinyfd_openFileDialog("Open Level File", "", 1, editor_dialog_filters, NULL, 0);
  if(!full_filename)
  {
    return 1; //canceled
  }

  char filename[30];

  char* match = strrchr(full_filename, '/'); //TODO: correct for Windows systems.
  if(match != NULL)
  {
    //match is now /level.bin
    strncpy(filename, match + 1, sizeof(filename) - 1);
  }

  char* match2 = strstr(full_filename, filename);
  if(match2 != NULL)
  {
    strncpy(match2, "", 30);
  }

  tilemap_destroy(editor->tilemap);
  editor->tilemap = tilemap_read_from_file(full_filename, filename); //full is now directory.
  editor->current_screen = EDITOR_SCREEN_EDIT;
  editor->current_tile_id = 0;

  editor->directory = malloc(sizeof(char) * strlen(full_filename) + 1);
  memset(editor->directory, 0, strlen(full_filename));
  strncpy(editor->directory, full_filename, strlen(full_filename) + 1);
  printf("%s\n", editor->directory);

  editor->filename = malloc(sizeof(char) * strlen(filename) + 1);
  memset(editor->filename, 0, strlen(filename));
  strncpy(editor->filename, filename, strlen(filename) + 1);

  SDL_Delay(50); //artificial delay

  return 0;
}

void editor_extract_directory_from_path(const char* full_path, char* out_directory, char* out_filename)
{
}

int editor_save_level(level_editor_t* editor)
{
  if(editor->directory == NULL || editor->filename == NULL)
  {
    const char* full_filename = tinyfd_saveFileDialog("Save Level File", editor->tilemap->map_name, 1, editor_dialog_filters, NULL);

    if(!full_filename)
    {
      return 0;
    }

    tilemap_write_to_file(full_filename, editor->tilemap);

    char filename[30];

    char* match = strrchr(full_filename, '/'); //TODO: correct for Windows systems.
    if(match != NULL)
    {
      //match is now /level.bin
      strncpy(filename, match + 1, sizeof(filename) - 1);
    }

    char* match2 = strstr(full_filename, filename);
    if(match2 != NULL)
    {
      strncpy(match2, "", 30);
    }

    editor->directory = malloc(sizeof(char) * strlen(full_filename));
    memmove(editor->directory, full_filename, strlen(full_filename));
    printf("%s\n", editor->directory);

    editor->filename = malloc(sizeof(char) * strlen(filename) + 1);
    memset(editor->filename, 0, strlen(filename));
    strncpy(editor->filename, filename, strlen(filename) + 1);

    SDL_Delay(50);
    printf("Saved to %s%s\n", editor->directory, editor->filename);
    return 0;
  }
  else
  {
    char buffer[256];
    strcat(buffer, editor->directory);
    strcat(buffer, editor->filename);
    tilemap_write_to_file(buffer, editor->tilemap);
    printf("Saved to %s\n", buffer);
    return 0;
  }

  return 1;
}

void editor_draw_editor(level_editor_t* editor)
{
  SDL_SetRenderDrawColor(kSdlRenderer, 20, 20, 20, 255);
  SDL_RenderClear(kSdlRenderer);
  SDL_SetRenderDrawColor(kSdlRenderer , 0, 0, 0, 255);

  tilemap_draw(editor->tilemap, kCamera);

  /**
  Get Selection Area
  */
  vector_t location = input_mouse_to_world(kCamera);
  SDL_Rect rect;

  rect.x = ((floor(location.x / 32) * 32) + kCamera->x);
  rect.y = ((floor(location.y / 32) * 32) + kCamera->y);
  rect.w = 32;
  rect.h = 32;
  /**
  End Get Selection Area
  */

  /**
  Draw Current Tile
  */

  vector_t sheet_location = tile_get_location_by_id(editor->current_tile_id);
  sprite_set_angle(editor->tilemap->tileset, editor->current_tile_angle);
  sprite_draw_source(editor->tilemap->tileset, rect.x, rect.y, sheet_location.x, sheet_location.y, 32, 32);
  sprite_set_angle(editor->tilemap->tileset, 0);
  /**
  End current tile
  */

  /**
  Draw Selection
  */
  SDL_RenderDrawRect(kSdlRenderer, &rect);
  /**
  End Draw Selection
  */

  SDL_RenderPresent(kSdlRenderer);
}

void editor_draw_pick_tile(level_editor_t* editor)
{
  SDL_SetRenderDrawColor(kSdlRenderer, 40, 40, 40, 10);
  SDL_RenderClear(kSdlRenderer);

  sprite_draw(editor->tilemap->tileset);

  /**
  Draw Selection
  */
  {
    vector_t location = input_mouse_to_world(NULL);
    SDL_Rect rect;

    rect.x = ((floor(location.x / 32) * 32)); //+ kCamera->x);
    rect.y = ((floor(location.y / 32) * 32)); //+ kCamera->y);
    rect.w = 32;
    rect.h = 32;

    SDL_RenderDrawRect(kSdlRenderer, &rect);
  }
  /**
  End Draw Selection
  */

  SDL_RenderPresent(kSdlRenderer);
}

void editor_draw(level_editor_t* editor)
{
  switch(editor->current_screen)
  {
  case EDITOR_SCREEN_EDIT:
    editor_draw_editor(editor);
    break;
  case EDITOR_SCREEN_PICK_TILE:
    editor_draw_pick_tile(editor);
    break;
  default:
    editor_draw_editor(editor);
    break;
  }
}

void editor_handle_input(level_editor_t* editor)
{
  if(kSdlEvent.type == SDL_KEYDOWN)
  {
    const Uint8* keyboard_state;
    keyboard_state = SDL_GetKeyboardState(NULL);
    if(keyboard_state[CONTROL_SCANCODE] && keyboard_state[SDL_SCANCODE_O]) //open
    {
      editor_load_level(editor);
    }
    else if(keyboard_state[CONTROL_SCANCODE] && keyboard_state[SDL_SCANCODE_S]) //save
    {
      editor_save_level(editor);
    }
  }
  else if(kSdlEvent.type == SDL_KEYUP)
  {
    if(kSdlEvent.key.keysym.sym == SDLK_j)
    {
      editor->current_screen = EDITOR_SCREEN_EDIT;
    }
    else if(kSdlEvent.key.keysym.sym == SDLK_k)
    {
      editor->current_screen = EDITOR_SCREEN_PICK_TILE;
    }
  }

  if(input_is_button_just_pressed(INPUT_BUTTON_ANGLE_INCREASE)) //decrease angle
  {
    editor->current_tile_angle += 90;
    if(editor->current_tile_angle > 360)
      editor->current_tile_angle = 0;
  }

  if(editor->current_screen == EDITOR_SCREEN_PICK_TILE)
  {
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      vector_t location = input_mouse_to_world(NULL);
      int sx, sy;
      sx = floor(location.x / 32);
      sy = floor(location.y / 32);

      editor->current_tile_id = tile_get_id_by_location(sx, sy);
      printf("id: %d\n", editor->current_tile_id);
      editor->current_screen = EDITOR_SCREEN_EDIT;
      editor->current_tile_angle = 0;
      SDL_Delay(100);
      SDL_PumpEvents();
    }
  }

  if(editor->current_screen == EDITOR_SCREEN_EDIT)
  {
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      vector_t location = input_mouse_to_world(kCamera);
      int sx, sy;
      sx = floor(location.x / 32);
      sy = floor(location.y / 32); //to normalize

      if(sx >= 0 && sx < editor->tilemap->width * 32)
      {
        if(sy >= 0 && sy < editor->tilemap->height * 32)
        {
          int index = sx * editor->tilemap->height + sy;
          tile_t tile = editor->tilemap->tiles[index];
          tile.id = editor->current_tile_id;
          tile.angle = editor->current_tile_angle;
          editor->tilemap->tiles[index] = tile;
        }
      }
    }
  }
}

void editor_update(level_editor_t* editor)
{
  input_update();
  editor_handle_input(editor);

  tilemap_update(editor->tilemap, kCamera);
  player_update(kPlayer);
}

#endif //SDL_VERS
