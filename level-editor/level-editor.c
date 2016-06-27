#include "level-editor.h"

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

  editor->tilemap = tilemap_create(32, 32); //32 x 32 map by default.

  editor->directory = "./";
  editor->filename = "kek.bin";

  editor->current_screen = EDITOR_SCREEN_EDIT;
  editor->current_tile_id = 0;

  /**
  For retrieving graphical mode info
  */
  tinyfd_inputBox("tinyfd_query", NULL, NULL);

  printf("using tinyfiledialogs\nCopyright (c) 2014 - 2016 Guillaume Vareille http://ysengrin.com http://tinyfiledialogs.sourceforge.net");
  printf("\n\ngraphics mode: %s\n", tinyfd_response);

  //tinyfd_messageBox("Editor Initiated", "Initiated blank 32x32 level.", "ok", "information", 0);

  return editor;
}

void editor_destroy(level_editor_t* editor)
{
  tilemap_destroy(editor->tilemap);
  free(editor);
}

int editor_load_level(level_editor_t* editor, const char* dir, const char* filename)
{

}

int editor_save_level(level_editor_t* editor, const char* dir, const char* filename)
{

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
  vector_t location = input_mouse_to_world(kInput, kCamera);
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
  sprite_draw_source(editor->tilemap->tileset, rect.x, rect.y, sheet_location.x, sheet_location.y, 32, 32);
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
  SDL_SetRenderDrawColor(kSdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(kSdlRenderer);

  sprite_draw(editor->tilemap->tileset);

  /**
  Draw Selection
  */
  {
    vector_t location = input_mouse_to_world(kInput, kCamera);
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

void editor_update(level_editor_t* editor)
{
  input_update(kInput);

  if(kSdlEvent.type == SDL_KEYUP)
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

  if(editor->current_screen == EDITOR_SCREEN_PICK_TILE)
  {
    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      vector_t location = input_mouse_to_world(kInput, NULL);
      int sx, sy;
      sx = floor(location.x / 32);
      sy = floor(location.y / 32);

      editor->current_tile_id = tile_get_id_by_location(sx, sy);
      printf("id: %d\n", editor->current_tile_id);
      editor->current_screen = EDITOR_SCREEN_EDIT;
    }
  }

  tilemap_update(editor->tilemap, kCamera);
  player_update(kPlayer);
}
