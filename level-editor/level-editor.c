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

  /**
  For retrieving graphical mode info
  */
  tinyfd_inputBox("tinyfd_query", NULL, NULL);

  printf("using tinyfiledialogs\nCopyright (c) 2014 - 2016 Guillaume Vareille http://ysengrin.com http://tinyfiledialogs.sourceforge.net");
  printf("\n\ngraphics mode: %s\n", tinyfd_response);

  tinyfd_messageBox("Editor Initiated", "Initiated blank 32x32 level.", "ok", "information", 0);

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

void editor_draw(level_editor_t* editor)
{
  SDL_RenderClear(kSdlRenderer);

  tilemap_draw(editor->tilemap, kCamera);

  /**
  Draw Selection
  */
  {
    vector_t location = input_mouse_to_world(kInput, kCamera);
    SDL_Rect rect;

    rect.x = ((floor(location.x / 32) * 32) + kCamera->x);
    rect.y = ((floor(location.y / 32) * 32) + kCamera->y);
    rect.w = 32;
    rect.h = 32;

    SDL_RenderDrawRect(kSdlRenderer, &rect);
  }
  /**
  End Draw Selection
  */

  SDL_RenderPresent(kSdlRenderer);
}

void editor_update(level_editor_t* editor)
{
  tilemap_update(editor->tilemap, kCamera);
  player_update(kPlayer);
  input_update(kInput);
}
