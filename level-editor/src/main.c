#include <stdio.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "../../map/tilemap.h"

int initialize_SDL()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return 1;
  }

  kSdlWindow = SDL_CreateWindow("Level Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
  if(kSdlWindow == NULL)
  {
    printf("Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  kSdlRenderer = SDL_CreateRenderer(kSdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(kSdlRenderer == NULL)
  {
    printf("Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(kSdlWindow);
    SDL_Quit();
    return 1;
  }

  return 0;
}

int shutdown_SDL()
{
  if(kSdlRenderer != NULL)
    SDL_DestroyRenderer(kSdlRenderer);

  if(kSdlWindow != NULL)
    SDL_DestroyWindow(kSdlWindow);

  return 0;
}

int main(void)
{
  if(initialize_SDL() > 0)
  {
    shutdown_SDL();
  }

  while(1)
  {
    SDL_RenderClear(kSdlRenderer);
    SDL_RenderPresent(kSdlRenderer);
    SDL_Delay(1000);
  }

  return shutdown_SDL();
}
