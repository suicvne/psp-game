#include "text.h"

SDL_Color text_standard_text_color = { 0, 0, 0, 255 }; //black

int text_render_text(const char* text, int x, int y)
{
  #if PSP

  oslDrawStringf(x, y, text);

  #elif SDL_VERS

  //wew
  SDL_Surface* textSurface = TTF_RenderText_Solid(kSdlFont, text, text_standard_text_color);

  if(textSurface != NULL)
  {
    SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(kSdlRenderer, textSurface);
    if(tempTexture != NULL)
    {
      SDL_Rect src_rect = {
        0, 0, textSurface->w, textSurface->h
      };

      SDL_Rect dst_rect = {
        x, y, src_rect.w, src_rect.h
      };

      SDL_RenderCopy(kSdlRenderer, tempTexture, &src_rect, &dst_rect);

      SDL_DestroyTexture(tempTexture);
    }
    SDL_FreeSurface(textSurface);
  }

  #endif

  return 0;
}
