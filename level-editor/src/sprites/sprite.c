#include "sprite.h"

sprite_t* sprite_create(const char* sprite_path, SPRITE_TYPE type)
{
  sprite_t* sprite = malloc(sizeof(sprite_t));
  sprite->frames = 0;
  sprite->currentframe = 0;
  sprite->yframeoffset = 0;
  sprite->rectangle.x = 0;
  sprite->rectangle.y = 0;
  sprite->rectangle.w = 0;
  sprite->rectangle.h = 0;

  if(type == SPRITE_TYPE_PNG)
  {
    #ifdef PSP
    sprite->image = oslLoadImageFilePNG(sprite_path, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    #else
    sprite->image = IMG_LoadTexture(kSdlRenderer, sprite_path);
    if(sprite->image == NULL)
    {
      reportFatalError(IMG_GetError());
    }
    #endif
    printf("png sprite with path '%s' has pointer value %p\n", sprite_path, (void*)sprite->image);
  }
  else if(type == SPRITE_TYPE_JPEG)
  {
    #ifdef PSP
    sprite->image = oslLoadImageFileJPG(sprite_path, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    #endif
    printf("jpeg sprite with path '%s' has pointer value %p\n", sprite_path, (void*)sprite->image);
  }
  else
  {
    reportFatalError("Wrong given type!");
    return NULL;
  }

  return sprite;
}

void sprite_destroy(sprite_t* sprite)
{
  assert(sprite->image != NULL);
  free(sprite);
  //TODO: proper resource freeing for SDL and OSLib
}

void sprite_set_angle(sprite_t* sprite, int angle)
{
  #ifdef PSP
  sprite->image->angle = angle;
  #else
  sprite->angle = angle;
  #endif
}

void sprite_draw(sprite_t* sprite)
{
  /*
  int SDL_RenderCopyEx(SDL_Renderer*          renderer,
                     SDL_Texture*           texture,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)
  */

  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image, sprite->rectangle.x, sprite->rectangle.y);
  #else

  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x;
  dst_rect.y = sprite->rectangle.y;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );

  #endif
}

void sprite_draw_offset(sprite_t* sprite, int x_offset, int y_offset)
{
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image, sprite->rectangle.x + x_offset, sprite->rectangle.y + y_offset);
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x + x_offset;
  dst_rect.y = sprite->rectangle.y + y_offset;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

/**
Draws a sprite given a camera.
*/
void sprite_draw_camera(sprite_t* sprite, const camera_t camera)
{
  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(
    sprite->image,
    sprite->rectangle.x + camera.x,
    sprite->rectangle.y + camera.y
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x + camera.x;
  dst_rect.y = sprite->rectangle.y + camera.y;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

void sprite_draw_camera_source(sprite_t* sprite, const camera_t camera, int x, int y, int sx, int sy, int w, int h)
{
  printf("sprite_draw_camera_source called\n");

  #ifdef PSP
  if(sprite == NULL)
  {
    oslFatalError("passed sprite was null");
  }
  oslSetImageTileSize(sprite->image, sx, sy, w, h);

  oslDrawImageXY(
    sprite->image,
    x + camera.x,
    y + camera.y
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = w;
  src_rect.h = h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x + camera.x;
  dst_rect.y = sprite->rectangle.y + camera.y;
  dst_rect.w = w;
  dst_rect.h = h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

/**
Draws a sprite given a camera with a factor at which to move it for say, slower movement.
*/
void sprite_draw_camera_factor(sprite_t* sprite, const camera_t camera, float movement_factor)
{
  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    sprite->rectangle.x + (camera.x * movement_factor),
    sprite->rectangle.y + (camera.y * movement_factor)
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x + (camera.x * movement_factor);
  dst_rect.y = sprite->rectangle.y + (camera.y * movement_factor);
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

void sprite_draw_camera_factor_offset(sprite_t* sprite, const camera_t camera, float movement_factor, int x_offset, int y_offset)
{
  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    sprite->rectangle.x + (camera.x * movement_factor) + x_offset,
    sprite->rectangle.y + (camera.y * movement_factor) + y_offset
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = sprite->rectangle.x + (camera.x * movement_factor) + x_offset;
  dst_rect.y = sprite->rectangle.y + (camera.y * movement_factor) + y_offset;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

void sprite_draw_camera_pointer_factor_offset(sprite_t* sprite, const camera_t camera, int x, int y, float movement_factor, int x_offset, int y_offset)
{
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  #ifdef PSP
  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    x + (camera.x * movement_factor) + x_offset,
    y + (camera.y * movement_factor) + y_offset
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = sprite->rectangle.w;
  src_rect.h = sprite->rectangle.h;

  SDL_Rect dst_rect;
  dst_rect.x = x + (camera.x * movement_factor) + x_offset;
  dst_rect.y = y + (camera.y * movement_factor) + y_offset;
  dst_rect.w = src_rect.w;
  dst_rect.h = src_rect.h;

  SDL_RenderCopyEx(kSdlRenderer,
    sprite->image,
    &src_rect,
    &dst_rect,
    sprite->angle,
    NULL,
    SDL_FLIP_NONE
  );
  #endif
}

void sprite_update(sprite_t* sprite)
{
  sprite->elapsedticks++;
  if(sprite->elapsedticks > sprite->frametime)
  {
    sprite->elapsedticks = 0;
    sprite->currentframe++;
    if(sprite->currentframe > sprite->frames - 1) //subtract one because it's index based
    {
      sprite->currentframe = 0;
    }
  }
}
