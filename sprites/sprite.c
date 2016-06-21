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
    sprite->image = oslLoadImageFilePNG(sprite_path, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    printf("png sprite with path '%s' has pointer value %p\n", sprite_path, (void*)sprite->image);
  }
  else if(type == SPRITE_TYPE_JPEG)
  {
    sprite->image = oslLoadImageFileJPG(sprite_path, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    printf("jpeg sprite with path '%s' has pointer value %p\n", sprite_path, (void*)sprite->image);
  }
  else
  {
    oslFatalError("Wrong given type!");
    return NULL;
  }

  return sprite;
}

void sprite_destroy(sprite_t* sprite)
{
  assert(sprite->image != NULL);
  free(sprite);
}

void sprite_draw(sprite_t* sprite)
{
  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image, sprite->rectangle.x, sprite->rectangle.y);
}

void sprite_draw_offset(sprite_t* sprite, int x_offset, int y_offset)
{
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image, sprite->rectangle.x + x_offset, sprite->rectangle.y + y_offset);
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

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(
    sprite->image,
    sprite->rectangle.x + camera.x,
    sprite->rectangle.y + camera.y
  );
}

void sprite_draw_camera_source(sprite_t* sprite, const camera_t camera, int x, int y, int sx, int sy, int w, int h)
{
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

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    sprite->rectangle.x + (camera.x * movement_factor),
    sprite->rectangle.y + (camera.y * movement_factor)
  );
}

void sprite_draw_camera_factor_offset(sprite_t* sprite, const camera_t camera, float movement_factor, int x_offset, int y_offset)
{
  //src x and src y from the sheet
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    sprite->rectangle.x + (camera.x * movement_factor) + x_offset,
    sprite->rectangle.y + (camera.y * movement_factor) + y_offset
  );
}

void sprite_draw_camera_pointer_factor_offset(sprite_t* sprite, const camera_t camera, int x, int y, float movement_factor, int x_offset, int y_offset)
{
  int sx, sy;
  sx = (sprite->currentframe * sprite->rectangle.w);
  sy = (sprite->yframeoffset * sprite->rectangle.h);

  if(sprite->frames > 0)
    oslSetImageTileSize(sprite->image, sx, sy, sprite->rectangle.w, sprite->rectangle.h);

  oslDrawImageXY(sprite->image,
    x + (camera.x * movement_factor) + x_offset,
    y + (camera.y * movement_factor) + y_offset
  );
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