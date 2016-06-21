#include <stdio.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "map/tilemap.h"

int initialize_SDL()
{
  #ifdef SDL_VERS
  printf("running SDL backend\n");
  #endif

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

int update_events(SDL_Event* event)
{
  while(SDL_PollEvent(event))
  {
    if(event->type == SDL_QUIT)
    {
      kQuit = 1;
    }
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

  tilemap_t* map = tilemap_create(32, 32);
  printf("map: %s (%d x %d)\n", map->map_name, map->width, map->height);

  kQuit = 0;
  SDL_Event mainEvent;

  while(!kQuit)
  {
    update_events(&mainEvent);

    SDL_RenderClear(kSdlRenderer);
    SDL_RenderPresent(kSdlRenderer);
  }

  tilemap_destroy(map);
  return shutdown_SDL();
}
