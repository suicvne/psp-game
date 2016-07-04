#include "text.h"

#if SDL_VERS
SDL_Color text_standard_text_color = { 255, 255, 255, 255 }; //white
SDL_Color text_shadow_text_color = { 0, 0, 0, 255 }; //black
#endif

int text_render_text(const char* text, int x, int y)
{
  #if PSP

  oslDrawStringf(x, y, text);

  #elif SDL_VERS

  //wew
  SDL_Surface* shadowSurface = TTF_RenderText_Solid(kSdlFont, text, text_shadow_text_color);
  SDL_Surface* textSurface = TTF_RenderText_Solid(kSdlFont, text, text_standard_text_color);

  if(textSurface != NULL)
  {
    SDL_Texture* tempShadowTexture = SDL_CreateTextureFromSurface(kSdlRenderer, shadowSurface);
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(kSdlRenderer, textSurface);
    if(tempTexture != NULL)
    {
      SDL_Rect src_rect = {
        0, 0, textSurface->w, textSurface->h
      };

      SDL_Rect dst_rect = {
        x, y, src_rect.w, src_rect.h
      };

      SDL_Rect dst_rect_shadow = {
        x - 1, y + 1, src_rect.w, src_rect.h
      };

      SDL_RenderCopy(kSdlRenderer, tempShadowTexture, &src_rect, &dst_rect_shadow);
      SDL_RenderCopy(kSdlRenderer, tempTexture, &src_rect, &dst_rect);

      SDL_DestroyTexture(tempTexture);
      SDL_DestroyTexture(tempShadowTexture);
    }
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(shadowSurface);
  }

  #endif

  return 0;
}

int text_get_width(const char* text)
{
  #if PSP
  return oslGetStringWidth(text);
  #elif SDL_VERS
  return 0; //TODO
  #endif
}