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

vector_t input_mouse_to_world(camera_t* camera)
{
  int window_width, window_height;
  SDL_GetWindowSize(kSdlWindow, &window_width, &window_height);

  float w_scale;
  float h_scale;
  get_scales(&w_scale, &h_scale);

  vector_t value;
  if(camera != NULL)
  {
    value.x = ((int)-camera->x) + (input_current_frame.mouse_x) / w_scale;
    value.y = ((int)-camera->y) + (input_current_frame.mouse_y) / h_scale;
  }
  else
  {
    value.x = (input_current_frame.mouse_x) / w_scale;
    value.y = (input_current_frame.mouse_y) / h_scale;
  }

  return value;
}
#endif

void input_begin_frame()
{
  input_last_frame = input_current_frame; //pls copy

  input_current_frame.button_interact = 0; //zero off current frame
  input_current_frame.button_use_item = 0;
  input_current_frame.button_hotbar_next = 0;
  input_current_frame.button_hotbar_back = 0;

  input_current_frame.button_dpad_up = 0;
  input_current_frame.button_dpad_down = 0;
  input_current_frame.button_dpad_left = 0;
  input_current_frame.button_dpad_right = 0;

  input_current_frame.button_inventory_open = 0;

  #if SDL_VERS
  input_current_frame.button_angle_increase = 0;
  #endif
}

int input_is_button_just_pressed(INPUT_BUTTON_TYPES button_type)
{
  //TODO: use arrays and do other cool stuff to make this more practical.
  if(button_type == INPUT_BUTTON_INTERACT)
  {
    if(input_current_frame.button_interact && input_last_frame.button_interact == 0)
    {
      return 1;
    }
  }
  else if(button_type == INPUT_BUTTON_USE_ITEM)
  {
    if(input_current_frame.button_use_item && input_last_frame.button_use_item == 0)
      return 1;
  }
  else if(button_type == INPUT_BUTTON_HOTBAR_NEXT)
  {
    if(input_current_frame.button_hotbar_next && input_last_frame.button_hotbar_next == 0)
      return 1;
  }
  else if(button_type == INPUT_BUTTON_HOTBAR_BACK)
  {
    if(input_current_frame.button_hotbar_back && input_last_frame.button_hotbar_back == 0)
      return 1;
  }
  else if(button_type == INPUT_BUTTON_DUP)
  {
    if(input_current_frame.button_dpad_up && input_last_frame.button_dpad_up == 0)
      return 1; 
  }
  else if(button_type == INPUT_BUTTON_DDOWN)
  {
    if(input_current_frame.button_dpad_down && input_last_frame.button_dpad_down == 0)
      return 1; 
  }
  else if(button_type == INPUT_BUTTON_DLEFT)
  {
    if(input_current_frame.button_dpad_left && input_last_frame.button_dpad_left == 0)
      return 1; 
  }
  else if(button_type == INPUT_BUTTON_DRIGHT)
  {
    if(input_current_frame.button_dpad_right && input_last_frame.button_dpad_right == 0)
      return 1; 
  }
  else if(button_type == INPUT_BUTTON_ANGLE_INCREASE)
  {
    #if SDL_VERS //USED FOR LEVEL EDITOR ONLY
    if(input_current_frame.button_angle_increase && input_last_frame.button_angle_increase == 0)
      return 1;
    #endif
  }
  else if(button_type == INPUT_BUTTON_INVENTORY_OPEN)
  {
    if(input_current_frame.button_inventory_open && input_last_frame.button_inventory_open == 0)
      return 1;
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
      case SDLK_k:
      case SDLK_SPACE:
        input_current_frame.button_interact = 1;
        break;
      case SDLK_RIGHTBRACKET: //editor angle
        input_current_frame.button_angle_increase = 1;
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
    if(pad.Buttons & PSP_CTRL_SQUARE)
    {
      input_current_frame.button_use_item = 1;
    }
    if(pad.Buttons & PSP_CTRL_LTRIGGER)
    {
      input_current_frame.button_hotbar_back = 1;
    }
    if(pad.Buttons & PSP_CTRL_RTRIGGER)
    {
      input_current_frame.button_hotbar_next = 1;
    }
    if(pad.Buttons & PSP_CTRL_TRIANGLE)
      input_current_frame.button_inventory_open = 1;

    if(pad.Buttons & PSP_CTRL_UP)
      input_current_frame.button_dpad_up = 1;
    else if(pad.Buttons & PSP_CTRL_DOWN)
      input_current_frame.button_dpad_down = 1;
    else if(pad.Buttons & PSP_CTRL_LEFT)
      input_current_frame.button_dpad_left = 1;
    else if(pad.Buttons & PSP_CTRL_RIGHT)
      input_current_frame.button_dpad_right = 1;
  }
  #endif
}
