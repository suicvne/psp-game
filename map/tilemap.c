#include "tilemap.h"

tilemap_t* tilemap_create(int width, int height)
{
  tilemap_t* tilemap = malloc(sizeof(tilemap_t));
  int total_tiles = width * height;
  tilemap->tiles = malloc(sizeof(tile_t) * total_tiles);
  tilemap->surrounding_tile_id = 1; //stone
  printf("allocated %d for map\n", sizeof(tile_t) * total_tiles);

  int x, y;
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      tilemap->tiles[x * height + y].id = 0;
      tilemap->tiles[x * height + y].tile_type = TILE_TYPE_PASSABLE;
    }
  }

  tilemap->width = width;
  tilemap->height = height;

  tilemap->map_name = "Test Map";

  return tilemap;
}

void tilemap_destroy(tilemap_t* map)
{
  assert(map != NULL);
  assert(map->tiles != NULL);
  free(map->tiles);
  free(map);
}

void tilemap_update(tilemap_t* map, const camera_t cam)
{/*TODO*/}

void tilemap_draw(tilemap_t* map, const camera_t cam, sprite_t* tileset)
{
  /*
  Get bounds for drawing
  */
  int min_x, max_x, min_y, max_y, x_iter, y_iter;

  camera_get_index_bounds(cam, map, &min_x, &max_x, &min_y, &max_y);

  for(x_iter = min_x; x_iter < max_x; x_iter++)
  {
    for(y_iter = min_y; y_iter < max_y; y_iter++)
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
        sprite_draw_camera_source(tileset, cam, x_iter * 32, y_iter * 32, sheet_location.x, sheet_location.y, 32, 32);
      }
    }
  }
}

int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t camera)
{
  rectangle_t player_hitbox = camera_player_to_world_rectangle(camera);
    player_hitbox.w = 32; player_hitbox.h = 32;

  rectangle_t theoretical_level_bounds;
  theoretical_level_bounds.x = 32;
  theoretical_level_bounds.y = 32;
  theoretical_level_bounds.w = (map->width - 1) * 32;
  theoretical_level_bounds.h = (map->height - 1) * 32;

  if(rectangle_intersects(&player_hitbox, &theoretical_level_bounds))
    return 0;
  else
    return 1;
}

void camera_get_index_bounds(const camera_t camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y)
{
  int half_map_width, half_map_height;
  half_map_width = ((tilemap->width * 32) / 2);
  half_map_height = ((tilemap->height * 32) / 2);

  float corrected_x, corrected_y;
  corrected_x = -camera.x;
  corrected_y = -camera.y;

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
