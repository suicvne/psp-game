#include "tilemap.h"

#include "serializer.h"
#include "serialization_reader.h"

#include <stdlib.h>
#include <assert.h>

void report_fatal_error(char* placeholder)
{
    /*placeholder*/
    printf("%s\n", placeholder);
}

tilemap_t* tilemap_create(int width, int height, int allocate_texture)
{
  tilemap_t* tilemap = malloc(sizeof(tilemap_t));
  int total_tiles = width * height;
  tilemap->tiles = malloc(sizeof(tile_t) * total_tiles);
  tilemap->surrounding_tile_id = 1; //stone
  //printf("allocated %d for map\n", (int)(sizeof(tile_t) * total_tiles));
    if(allocate_texture){}
  int x, y;
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      int index = x * height + y;
      tilemap->tiles[index].id = 0;
      tilemap->tiles[index].id_layer2 = -1;
      tilemap->tiles[index].angle = 0;
      tilemap->tiles[index].tile_type = TILE_TYPE_PASSABLE;
    }
  }

  tilemap->width = width;
  tilemap->height = height;

  tilemap->tileset_path = "textures.png";
  tilemap->foreground_tileset_path = "NONE";
  tilemap->map_name = "Test Map";

  return tilemap;
}

void tilemap_destroy(tilemap_t* map)
{
  //TODO: proper checking and handling for this. I like assert though :'/
  assert(map != NULL);
  assert(map->tiles != NULL);
}

/*
void tilemap_draw(tilemap_t* map)
{
    //Keeping this older code here for reference during the creation of the editor.
  //TODO: move player drawing into here for proper layering

  Get bounds for drawing


  int min_x, max_x, min_y, max_y, x_iter, y_iter;

  camera_get_index_bounds(cam, map, &min_x, &max_x, &min_y, &max_y);

  for(x_iter = zeroize(min_x); x_iter < min(max_x, map->width); x_iter++)
  {
    for(y_iter = zeroize(min_y); y_iter < min(max_y, map->height); y_iter++)
    {
      int index = x_iter * map->height + y_iter;

      if(index < 0 || index > (map->height * map->width))
      {
        continue;
      } //skip NULL tiles
      else
      {
        tile_t tile = map->tiles[index];
        vector_t sheet_location = tile_get_location_by_id(tile.id);
        sprite_set_angle(map->tileset, tile.angle);
        if(tile.angle > 0)
        {
          int cx, cy;
          switch(tile.angle) //...i'm..not proud of this..
          {
            case 90:
            cx = 0; cy = 32;
            break;
            case 180:
            cx = 32; cy = 32;
            break;
            case 270:
            cx = 32; cy = 0;
            break;
            default:
            cx = 0; cy = 0;
            break;
          }
          sprite_set_center_point(map->tileset, cx, cy);
        }
        sprite_draw_camera_source(map->tileset, *cam, floor(x_iter * 32), floor(y_iter * 32), sheet_location.x, sheet_location.y, 32, 32);
        sprite_set_angle(map->tileset, 0);
        sprite_set_center_point(map->tileset, 0, 0);
      }
    }
  }
  */

  //oslSetAlpha(OSL_FX_ALPHA, RGBA(255, 127, 0, 255));
  //oslSetAlpha(OSL_FX_ALPHA, 255);
  //oslDrawFillRect(0, 0, 480, 272, RGBA(255, 255, 255, 255));

  //oslDrawFillRect(20, 20, 100, 100, RGBA(255, 127, 0, 255));

  //oslSetAlpha(OSL_FX_ALPHA, 255);

int tilemap_write_to_file(const char* filename, tilemap_t* map)
{
  int total_tiles = (map->width * map->height) + 1;
  int filesize = (sizeof(char) * 2) + //header (MS)
                  (sizeof(short)) + //VERSION
                  strlen(map->map_name) + //map name
                  strlen(map->tileset_path) + //tileset path
                  strlen(map->foreground_tileset_path) + //tileset foreground path
                  (sizeof(int) * 2) + // two ints for width and height
                  (sizeof(short) * total_tiles) + //id
                  (sizeof(short) * total_tiles) + //id_layer2
                  (sizeof(char) * total_tiles) + // the tiles in this level *2 for their rotation value too.
                  (sizeof(char) * total_tiles); // for their collision values

  char* buffer = malloc(sizeof(char) * filesize);
  int pointer = 0, i;
  serializer_write_char(buffer, &pointer, HEADER_0);
  serializer_write_char(buffer, &pointer, HEADER_1);

  serializer_write_short(buffer, &pointer, (short)VERSION);

  serializer_write_string(buffer, &pointer, map->map_name);
  serializer_write_string(buffer, &pointer, map->tileset_path);
  if(map->foreground_tileset_path == NULL)
    serializer_write_string(buffer, &pointer, "NONE");
  else
    serializer_write_string(buffer, &pointer, map->foreground_tileset_path);

  serializer_write_int(buffer, &pointer, map->width);
  serializer_write_int(buffer, &pointer, map->height);

  for(i = 0; i < total_tiles; i++)
  {
    serializer_write_short(buffer, &pointer, map->tiles[i].id);
    serializer_write_short(buffer, &pointer, map->tiles[i].id_layer2);
    switch(map->tiles[i].angle)
    {
    case 0:
      serializer_write_char(buffer, &pointer, ((char)1));
      break;
    case 90:
      serializer_write_char(buffer, &pointer, ((char)2));
      break;
    case 180:
      serializer_write_char(buffer, &pointer, ((char)3));
      break;
    case 270:
      serializer_write_char(buffer, &pointer, ((char)4));
      break;
    default:
      serializer_write_char(buffer, &pointer, ((char)1)); //default to 0deg
      break;
    }
    serializer_write_char(buffer, &pointer, ((char)map->tiles[i].tile_type));
  }

  return serializer_write_to_file(buffer, filesize, filename);
}

int tilemap_verify_header(char* buffer, short version)
{
  char filler[32];

  if(buffer[0] == HEADER_0 && buffer[1] == HEADER_1)
    return 1;
  else
  {
    if(buffer[0] != HEADER_0 || buffer[1] != HEADER_1)
    {
      sprintf(filler, "Header mismatch in level file. (got %c%c; expected %c%c)\n",
        buffer[0], buffer[1],
        HEADER_0, HEADER_1
      );
      printf("%s", filler);
    }
    else if(version != VERSION)
    {
      if(version >= 4)
          return 1;
      else
      {
        sprintf(filler, "Version mismatch in level file. (got %d; expected %d)\n", version, VERSION);
        printf("%s", filler);
      }
    }
  }
  return 0;
}

tilemap_t* tilemap_read_from_file(const char* filename)
{
  //char combined_filename[32];
  //sprintf(combined_filename, "%s/%s", directory, filename);
  //char* combined_filename = filename;

  int file_size = serializer_get_file_size(filename);
  if(file_size > 0)
  {
    char* buffer = malloc(sizeof(char) * file_size);
    int pointer = 0;

    serializer_read_from_file(buffer, file_size, filename);

    char HEADER[2] = {0};
    HEADER[0] = serializer_read_char(buffer, &pointer);
    HEADER[1] = serializer_read_char(buffer, &pointer);
    short version = serializer_read_short(buffer, &pointer);

    printf("level version: %d\n", version);

    if(tilemap_verify_header(HEADER, version))
    {
      char* map_name = serializer_read_string(buffer, &pointer);
      char* tileset_path = serializer_read_string(buffer, &pointer);
      char* foreground_tileset_path = "NONE";
      if(version >= 5)
          foreground_tileset_path = serializer_read_string(buffer, &pointer);
      printf("tileset: %s\n", tileset_path);
      int width, height;
      width = serializer_read_int(buffer, &pointer);
      height = serializer_read_int(buffer, &pointer);

      tilemap_t* return_value = tilemap_create(width, height, 0);
      return_value->map_name = map_name;
      return_value->tileset_path = tileset_path;
      return_value->foreground_tileset_path = foreground_tileset_path;

      int total_tiles = width * height;
      int i = 0;
      for(i = 0; i < total_tiles; i++)
      {
        //read shorts for tiles
        short id = serializer_read_short(buffer, &pointer);
        short id_layer2 = -1;
        if(version >= 5)
            id_layer2 = serializer_read_short(buffer, &pointer);
        short angle = 0;
        unsigned char angle_char = serializer_read_char(buffer, &pointer);
        switch(angle_char)
        {
        case 1:
          angle = 0;
          break;
        case 2:
          angle = 90;
          break;
        case 3:
          angle = 180;
          break;
        case 4:
          angle = 270;
          break;
        }
        TILE_TYPE collision = ((TILE_TYPE)serializer_read_char(buffer, &pointer));

        /*if(i == 0)
        {
          printf("angle: %d\n", angle);
          printf("collision: %d\n", (int)collision);
        }*/
        if(id >= 0)
          return_value->tiles[i].id = id;
        if(id_layer2 > -1)
            return_value->tiles[i].id_layer2 = id_layer2;
        if(angle >= 0 || angle <= 360)
          return_value->tiles[i].angle = angle;

        return_value->tiles[i].tile_type = collision;
      }

      free(buffer);

      //char temp[60];
      //sprintf(temp, "res/%s", tileset_path);
      //sprite_set_center_point(return_value->tileset, 256 / 2, 256 / 2);
      //sprite_set_center_point(return_value->tileset, 256 / 2, 256 / 2);

      return return_value;
    }
    else
      return NULL;
  }
  else
    return NULL; //fnf

  return NULL;
}
