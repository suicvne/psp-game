#ifndef ___TEXT_H___
#define ___TEXT_H___

#include "../globals.h"

#if SDL_VERS

#include <SDL2/SDL_ttf.h>

extern SDL_Color text_standard_text_color;
extern SDL_Color text_shadow_text_color;

#elif PSP

#include <oslib/oslib.h>

#endif

int text_render_text(const char* text, int x, int y);
int text_get_width(const char* text);

#endif
