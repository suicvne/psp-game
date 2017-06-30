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
#if PSP
    sprite->image = oslLoadImageFilePNG(sprite_path, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	sprite->rectangle.w = sprite->image->sizeX;
	sprite->rectangle.h = sprite->image->sizeY;
#else
    sprite->image = IMG_LoadTexture(kSdlRenderer, sprite_path);
	SDL_QueryTexture(sprite->image, NULL, NULL, &sprite->rectangle.w, &sprite->rectangle.h);
	
    if(sprite->image == NULL)
    {
      report_fatal_error(IMG_GetError());
    }
#endif
    printf("png sprite with path '%s' has pointer value %p\n", sprite_path, (void*)sprite->image);
  }
  else if(type == SPRITE_TYPE_JPEG)
  {
	  report_fatal_error("JPEG Sprite types are no longer supported!\n");
  }
  else
  {
    report_fatal_error("Wrong given type!");
    return NULL;
  }

  #if SDL_VERS
  sprite->angle = 0; //no rotation
  sprite->center.x = 0;
  sprite->center.y = 0; //top left
  #endif

  return sprite;
}

void sprite_destroy(sprite_t* sprite)
{
  assert(sprite->image != NULL);
  printf("destroying sprite with pointer %p\n", ((void*)sprite));

  #if PSP
  oslDeleteImage(sprite->image);
  #elif SDL_VERS
  SDL_DestroyTexture(sprite->image);
  #endif

  free(sprite);
}

void sprite_set_angle(sprite_t* sprite, int angle)
{
  #if PSP
  sprite->image->angle = angle;
  #else
  sprite->angle = angle;
  #endif
}

void sprite_set_center_point(sprite_t* sprite, int cx, int cy)
{
  #if PSP
  sprite->image->centerX = cx;
  sprite->image->centerY = cy;
  #else
  //sprite->center.x = cx;
  //sprite->center.y = cy;
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

  #if PSP
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

  #if PSP
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

  #if PSP
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
  #if PSP
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
  dst_rect.x = x + ((int)camera.x);
  dst_rect.y = y + ((int)camera.y);
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

void sprite_draw_source(sprite_t* sprite, int x, int y, int sx, int sy, int w, int h)
{
  #if PSP
  if(sprite == NULL)
  {
    oslFatalError("passed sprite was null");
  }
  oslSetImageTileSize(sprite->image, sx, sy, w, h);

  oslDrawImageXY(
    sprite->image,
    x,
    y
  );
  #else
  SDL_Rect src_rect;
  src_rect.x = sx;
  src_rect.y = sy;
  src_rect.w = w;
  src_rect.h = h;

  SDL_Rect dst_rect;
  dst_rect.x = x;
  dst_rect.y = y;
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

  #if PSP
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

  #if PSP
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

  #if PSP
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
