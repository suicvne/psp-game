#include "tilemap.h"

#include "../player/player.h"

int tilemap_load_lua_file(lua_State* L, const char* filename)
{
  if(kLevelEditorMode)
    return 0;

  int error;

  if(serializer_get_file_size(filename) > 0)
  {
    error = luaL_loadfile(L, filename);
    return error;
  }
  else
    return 1;
}

tile_t tilemap_get_tile_at(tilemap_t* tilemap, int x, int y)
{
  if(x >= 0 && x < tilemap->width - 1)
  {
    if(y >= 0 && y < tilemap->height - 1)
    {
      return tilemap->tiles[x * tilemap->height + y];
    }
  }
  else //trying to collide out of bounds. yikes
  {
    tile_t t;
    t.tile_type = TILE_TYPE_SOLID;
    return t;
  }
}

tilemap_t* tilemap_create(int width, int height, int allocate_texture)
{
  tilemap_t* tilemap = malloc(sizeof(tilemap_t));
  int total_tiles = width * height;
  tilemap->tiles = malloc(sizeof(tile_t) * total_tiles);
  tilemap->surrounding_tile_id = 1; //stone
  printf("allocated %d for map\n", (int)(sizeof(tile_t) * total_tiles));

  int x, y;
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      int index = x * height + y;
      tilemap->tiles[index].id = 0;
	  tilemap->tiles[index].id_layer2 = -1; //no secondary layer
      tilemap->tiles[index].angle = 0;
      tilemap->tiles[index].tile_type = TILE_TYPE_PASSABLE;
    }
  }

  tilemap->width = width;
  tilemap->height = height;

  tilemap->tileset_path = "textures.png";
  tilemap->foreground_tileset_path = NULL; //foreground tileset is purely optional
  char buffer[32];
  sprintf(buffer, "res/%s", tilemap->tileset_path);
  if(allocate_texture)
  {
    tilemap->tileset = sprite_create(buffer, SPRITE_TYPE_PNG);
    tilemap->tileset->rectangle.w = 256;
    tilemap->tileset->rectangle.h = 256;
    tilemap->tileset->rectangle.x = 0;
    tilemap->tileset->rectangle.y = 0;
    //sprite_set_center_point(tilemap->tileset, 256 / 2, 256 / 2);
  }

  tilemap->map_name = "Test Map";
  tilemap->lua_state = NULL;

  return tilemap;
}

void tilemap_destroy(tilemap_t* map)
{
  //TODO: proper checking and handling for this. I like assert though :'/
  assert(map != NULL);
  assert(map->tiles != NULL);
  assert(map->tileset != NULL);

  if(map->lua_state != NULL)
  {
    lua_close(map->lua_state);
  }

  #ifdef PSP
  free(map->tileset);
  if(map->foreground_tileset != NULL)
	  free(map->foreground_tileset);
  free(map->tiles);
  free(map);
  #else
  SDL_DestroyTexture(map->tileset->image);
  if(map->foreground_tileset != NULL)
	  SDL_DestroyTexture(map->foreground_tileset->image);
  #endif
}

void tilemap_update(tilemap_t* map, const camera_t* cam)
{
  //TODO: regular updating.

  /*
  Lua
  */

  if(map->lua_state != NULL && !kLevelEditorMode)
  {
    lua_getglobal(map->lua_state, "onUpdate");
    if(lua_isnil(map->lua_state, -1))
    {/*Function dne*/}
    else if(lua_pcall(map->lua_state, 0, 0, 0) != 0)
    {
      char buffer[64];
      sprintf(buffer, "Fatal Error during onUpdate: %s", lua_tostring(map->lua_state, -1));
      report_fatal_error(buffer);
      lua_pop(map->lua_state, 1);
    }
  }

  /*
  End Lua
  */
}

void tilemap_draw(tilemap_t* map, const camera_t* cam)
{
	int player_drawn = 0;
  //TODO: move player drawing into here for proper layering
  /*
  Get bounds for drawing
  */
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
		vector_t sheet_location_2 = tile_get_location_by_id(tile.id_layer2);
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
		//layer 1
        sprite_draw_camera_source(map->tileset, *cam, floor(x_iter * 32), floor(y_iter * 32), sheet_location.x, sheet_location.y, 32, 32);
		
		//player layer
		//if(!player_drawn)
		//{
			sprite_draw(kPlayer->main_sprite);
			player_drawn = 1;
			//}
		
		//layer 2
		if(sheet_location_2.x > -1) //x and y will both be -1 if layer 2 is to be skipped
		{
			sprite_draw_camera_source(map->tileset, *cam, floor(x_iter * 32), floor(y_iter * 32), sheet_location_2.x, sheet_location_2.y, 32, 32);
		}
		
        sprite_set_angle(map->tileset, 0);
        sprite_set_center_point(map->tileset, 0, 0);
      }
    }
  }

  //oslSetAlpha(OSL_FX_ALPHA, RGBA(255, 127, 0, 255));
  //oslSetAlpha(OSL_FX_ALPHA, 255);
  //oslDrawFillRect(0, 0, 480, 272, RGBA(255, 255, 255, 255));

  //oslDrawFillRect(20, 20, 100, 100, RGBA(255, 127, 0, 255));

  //oslSetAlpha(OSL_FX_ALPHA, 255);



  /*
  Lua
  */
  if(map->lua_state != NULL && !kLevelEditorMode)
  {
    lua_getglobal(map->lua_state, "onDraw");
    if(lua_isnil(map->lua_state, -1))
    {/*Function dne*/}
    else if(lua_pcall(map->lua_state, 0, 0, 0) != 0)
    {
      char buffer[256];
      sprintf(buffer, "Fatal Error during onDraw: %s", lua_tostring(map->lua_state, -1));
      report_fatal_error(buffer);
      lua_pop(map->lua_state, 1);
    }
  }
  /*
  End Lua
  */
}
/*
int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t* camera)
{
  rectangle_t player_hitbox = camera_player_to_world_rectangle(camera);
    player_hitbox.w = 32; player_hitbox.h = 32;

  rectangle_t theoretical_level_bounds;
  theoretical_level_bounds.x = 32;
  theoretical_level_bounds.y = 32;
  theoretical_level_bounds.w = (map->width) * 32;
  theoretical_level_bounds.h = (map->height) * 32;

  if(rectangle_intersects(&player_hitbox, &theoretical_level_bounds))
    return 0;
  else
    return 1;
}
*/

void camera_get_index_bounds(const camera_t* camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y)
{

  int half_map_width, half_map_height;
  half_map_width = ((tilemap->width * 32) / 2);
  half_map_height = ((tilemap->height * 32) / 2);


  float corrected_x, corrected_y;
  corrected_x = -camera->x;
  corrected_y = -camera->y;

  (*min_x) = floor(
    (corrected_x) / 32
  );
  (*max_x) = ceil(
    (corrected_x + SCREEN_WIDTH) / 32
  );
  (*min_y) = floor(
    (corrected_y) / 32
  );
  (*max_y) = ceil(
    (corrected_y + SCREEN_HEIGHT) / 32
  );
}

int tilemap_write_to_file(const char* filename, tilemap_t* map)
{
  int total_tiles = map->width * map->height;
  int filesize = (sizeof(char) * 2) + //header (MS)
                  (sizeof(short)) + //VERSION
                  strlen(map->map_name) + //map name
                  strlen(map->tileset_path) + //tileset path
					  //strlen(map->foreground_tileset_path) + //foreground tileset path
                  (sizeof(int) * 2) + // two ints for width and height
                  (sizeof(short) * total_tiles) +
                  (sizeof(char) * total_tiles) + // the tiles in this level *2 for their rotation value too.
                  (sizeof(char) * total_tiles); // for their collision values

  char* buffer = malloc(sizeof(char) * filesize);
  int pointer = 0, i;
  serializer_write_char(buffer, &pointer, HEADER_0);
  serializer_write_char(buffer, &pointer, HEADER_1);

  serializer_write_short(buffer, &pointer, (short)VERSION);

  serializer_write_string(buffer, &pointer, map->map_name);
  serializer_write_string(buffer, &pointer, map->tileset_path);
//    serializer_write_string(buffer, &pointer, map->foreground_tileset_path);

  serializer_write_int(buffer, &pointer, map->width);
  serializer_write_int(buffer, &pointer, map->height);

  for(i = 0; i < total_tiles; i++)
  {
    serializer_write_short(buffer, &pointer, map->tiles[i].id);
//	    serializer_write_short(buffer, &pointer, map->tiles[i].id_layer2);
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

  if(buffer[0] == HEADER_0 && buffer[1] == HEADER_1 && version == VERSION)
    return 1;
  else
  {
    if(buffer[0] != HEADER_0 || buffer[1] != HEADER_1)
    {
      sprintf(filler, "Header mismatch in level file. (got %c%c; expected %c%c)",
        buffer[0], buffer[1],
        HEADER_0, HEADER_1
      );
      report_fatal_error(filler);
    }
    else if(version != VERSION)
    {
      sprintf(filler, "Version mismatch in level file. (got %d; expected %d)", version, VERSION);
      report_fatal_error(filler);
    }
  }
  return 0;
}

void tilemap_report_lua_errors(lua_State* L, int status)
{
  if(status != 0)
  {
    char buffer[256];
    sprintf(buffer, "fatal error in Lua script: %s", lua_tostring(L, -1));
    report_fatal_error(buffer);
    lua_pop(L, 1); //remove error from Lua stack
  }
}

tilemap_t* tilemap_read_from_file(const char* directory, const char* filename)
{
  char combined_filename[32];
  char combined_filename_lua[32];
  sprintf(combined_filename, "%s/%s.bin", directory, filename);
  sprintf(combined_filename_lua, "%s/%s.lua", directory, filename);

  int file_size = serializer_get_file_size(combined_filename);
  if(file_size > 0)
  {
    char* buffer = malloc(sizeof(char) * file_size);
    int pointer = 0;

    serializer_read_from_file(buffer, file_size, combined_filename);

    char HEADER[2] = {0};
    HEADER[0] = serializer_read_char(buffer, &pointer);
    HEADER[1] = serializer_read_char(buffer, &pointer);
    short version = serializer_read_short(buffer, &pointer);

    if(tilemap_verify_header(HEADER, version))
    {
      char* map_name = serializer_read_string(buffer, &pointer);
      char* tileset_path = serializer_read_string(buffer, &pointer);
      printf("tileset: %s\n", tileset_path);
      int width, height;
      width = serializer_read_int(buffer, &pointer);
      height = serializer_read_int(buffer, &pointer);

      tilemap_t* return_value = tilemap_create(width, height, 0);
      return_value->map_name = map_name;
      return_value->tileset_path = tileset_path;

      int total_tiles = width * height;
      int i = 0;
      for(i = 0; i < total_tiles; i++)
      {
        //read shorts for tiles
        short id = serializer_read_short(buffer, &pointer);
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

        if(i == 0)
        {
          printf("angle: %d\n", angle);
          printf("collision: %d\n", (int)collision);
        }
        if(id >= 0)
          return_value->tiles[i].id = id;
        if(angle >= 0 || angle <= 360)
          return_value->tiles[i].angle = angle;

        return_value->tiles[i].tile_type = collision;
      }

      free(buffer);

      char temp[60];
      sprintf(temp, "./res/%s", tileset_path);
      return_value->tileset = sprite_create(temp, SPRITE_TYPE_PNG);
      //sprite_set_center_point(return_value->tileset, 256 / 2, 256 / 2);
      return_value->tileset->rectangle.w = 256;
      return_value->tileset->rectangle.h = 256;
      return_value->tileset->rectangle.x = 0;
      return_value->tileset->rectangle.y = 0;
      //sprite_set_center_point(return_value->tileset, 256 / 2, 256 / 2);


      /*
      Lua
      */
      if(!kLevelEditorMode)
      {
        printf("finding scripts..\n");
        return_value->lua_state = luaL_newstate();
        luaL_openlibs(return_value->lua_state);
	printf("attempting to load tilemap from '%s'...\n", combined_filename_lua);
        if(tilemap_load_lua_file(return_value->lua_state, combined_filename_lua) != 0) //non-zero means error
        {
          fprintf(stderr, "No Lua script assosciated with level. Freeing lua_state\n");
          lua_close(return_value->lua_state);
          return_value->lua_state = NULL;
        }
        else
        {
          printf("Loaded script successfully!\n");
          lua_map_register_functions(return_value->lua_state, return_value);
	  printf("lua pcall\n");
          int initError = lua_pcall(return_value->lua_state, 0, LUA_MULTRET, 0); //this call is necessary to "init" the script and index the globals i guess
	  printf("lua pcall complete: %d\n", initError);
          if(!initError)
          {
            lua_getglobal(return_value->lua_state, "onLoad");
            if(lua_isnil(return_value->lua_state, -1))
            {
              printf("No onLoad function, skipping.\n");
            }
            else
            {
              lua_pcall(return_value->lua_state, 0, 0, 0);
            }
          }
          else
          {
            char tempErrorBuffer[30];
            sprintf(tempErrorBuffer, "%s", lua_tostring(return_value->lua_state, -1));
            report_fatal_error(tempErrorBuffer);
            lua_pop(return_value->lua_state, 1);
          }
        }
      }
      /*
      End Lua
      */
      return return_value;
    }
    else
      return NULL;
  }
  else
    return NULL; //fnf

  return NULL;
}
