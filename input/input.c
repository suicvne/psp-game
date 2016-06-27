#include "input.h"

input_t* input_create()
{
  input_t* return_value = malloc(sizeof(input_t));

  return_value->analogue_input.x = 0.0f;
  return_value->analogue_input.y = 0.0f;

  return return_value;
}

void input_destroy(input_t* input)
{
  assert(input != NULL);
  free(input);
}

void input_update(input_t* input)
{
  //TODO: PSP input, keymapping, etc. ugh.
  #ifdef SDL_VERS
  while(SDL_PollEvent(&kSdlEvent))
  {
    switch(kSdlEvent.type)
    {
    case SDL_QUIT:
      kQuit = 1;
      break;
    case SDL_KEYDOWN:
      switch(kSdlEvent.key.keysym.sym)
      {
      case SDLK_w: //up
        input->analogue_input.y = -1.0f;
        break;
      case SDLK_a: //left
        input->analogue_input.x = -1.0f;
        break;
      case SDLK_s: //down
        input->analogue_input.y = 1.0f;
        break;
      case SDLK_d: //right
        input->analogue_input.x = 1.0f;
        break;
      }
      break;
    case SDL_KEYUP:
      switch(kSdlEvent.key.keysym.sym)
      {
      case SDLK_w: //up
        input->analogue_input.y = 0;
        break;
      case SDLK_a: //left
        input->analogue_input.x = 0;
        break;
      case SDLK_s: //down
        input->analogue_input.y = 0;
        break;
      case SDLK_d: //right
        input->analogue_input.x = 0;
        break;
      }
      break;
    }
  }
  #elif PSP
  SceCtrlData pad;
  sceCtrlPeekBufferPositive(&pad, 1);

  float padZeroX, padZeroY;
  padZeroX = ((pad.Lx) - 128) / 127.0f; //128 is directly in middle so i normalize it here to be closer to say unity (-1 to +1)
  padZeroY = ((pad.Ly) - 128) / 127.0f;
  
  input->analogue_input.x = padZeroX;
  input->analogue_input.y = padZeroY;
  #endif
}
