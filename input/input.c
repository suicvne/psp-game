#include "input.h"

input_t input_current_frame;
input_t input_last_frame;

/*
input_t* input_create()
{
  input_t* return_value = malloc(sizeof(input_t));

  return_value->analogue_input.x = 0.0f;
  return_value->analogue_input.y = 0.0f;

  #if SDL_VERS
  return_value->button_angle_decrease = 0;
  return_value->button_angle_increase = 0;
  #endif

  return return_value;
}
*/

#ifdef SDL_VERS
void get_scales(float* w, float* h)
{
  int ww, wh;
  SDL_GetWindowSize(kSdlWindow, &ww, &wh);

  *w = ww / SCREEN_WIDTH;
  *h = wh / SCREEN_HEIGHT;
}

vector_t input_mouse_to_world(input_t* input, camera_t* camera)
{
  int window_width, window_height;
  SDL_GetWindowSize(kSdlWindow, &window_width, &window_height);

  float w_scale;
  float h_scale;
  get_scales(&w_scale, &h_scale);

  vector_t value;
  if(camera != NULL)
  {
    value.x = ((int)-camera->x) + (input->mouse_x) / w_scale;
    value.y = ((int)-camera->y) + (input->mouse_y) / h_scale;
  }
  else
  {
    value.x = (input->mouse_x) / w_scale;
    value.y = (input->mouse_y) / h_scale;
  }

  return value;
}
#endif

void input_begin_frame()
{
  input_last_frame = input_current_frame; //pls copy

  input_current_frame.button_interact = 0; //zero off current frame
}

int input_is_button_just_pressed(INPUT_BUTTON_TYPES button_type)
{
  if(button_type == INPUT_BUTTON_INTERACT)
  {
    if(input_current_frame.button_interact && input_last_frame.button_interact == 0)
    {
      return 1;
    }
  }
  
  return 0;
}

void input_update()
{
  input_begin_frame();
  //TODO: keymapping, etc. ugh.
  #ifdef SDL_VERS
  while(SDL_PollEvent(&kSdlEvent))
  {
    switch(kSdlEvent.type)
    {
    case SDL_QUIT:
      kQuit = 1;
      break;
    case SDL_KEYDOWN:
      if(kSdlEvent.key.repeat == 0)
      {
        switch(kSdlEvent.key.keysym.sym)
        {
          case SDLK_LEFTBRACKET:
            input_current_frame.button_angle_decrease = 1;
          case SDLK_RIGHTBRACKET:
            input_current_frame.button_angle_increase = 1;
            break;
        }
      }
      switch(kSdlEvent.key.keysym.sym)
      {
      case SDLK_w: //up
        input_current_frame.analogue_input.y = -1.0f;
        break;
      case SDLK_a: //left
        input_current_frame.analogue_input.x = -1.0f;
        break;
      case SDLK_s: //down
        input_current_frame.analogue_input.y = 1.0f;
        break;
      case SDLK_d: //right
        input_current_frame.analogue_input.x = 1.0f;
        break;
      }
      break;
    case SDL_KEYUP:
      switch(kSdlEvent.key.keysym.sym)
      {
      case SDLK_w: //up
        input_current_frame.analogue_input.y = 0;
        break;
      case SDLK_a: //left
        input_current_frame.analogue_input.x = 0;
        break;
      case SDLK_s: //down
        input_current_frame.analogue_input.y = 0;
        break;
      case SDLK_d: //right
        input_current_frame.analogue_input.x = 0;
        break;
      }
      break;
    }
  }

  SDL_GetMouseState(&input_current_frame.mouse_x, &input_current_frame.mouse_y);

  #elif PSP
  SceCtrlData pad;
  sceCtrlPeekBufferPositive(&pad, 1);

  float padZeroX, padZeroY;
  padZeroX = ((pad.Lx) - 128) / 127.0f; //128 is directly in the middle so i normalize it here to be closer to say unity (-1 to +1)
  padZeroY = ((pad.Ly) - 128) / 127.0f;

  input_current_frame.analogue_input.x = padZeroX;
  input_current_frame.analogue_input.y = padZeroY;

  if(pad.Buttons != 0)
  {
    if(pad.Buttons & PSP_CTRL_CROSS) //x, default interact button
    {
      input_current_frame.button_interact = 1;
    }
  }
  #endif
}
