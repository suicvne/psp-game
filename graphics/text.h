#ifndef ___TEXT_H___
#define ___TEXT_H___

#if SDL_VERS

struct SDL_Color;
extern struct SDL_Color text_standard_text_color;
extern struct SDL_Color text_shadow_text_color;

#endif

int text_render_text(const char* text, int x, int y);
int text_render_char(const char character, int x, int y);
int text_get_width(const char* text);

#endif
