#include "tilemap.h"

tilemap::tilemap(int _width, int _height) : width(_width), height(_height)
{
    int total_tiles = width * height;

    this->tiles = (tile*)malloc(sizeof(tile) * total_tiles); //allocate the memory needed for the level array
    this->surrounding_tile_id = 1; //stone tile to surround the map, not worried about this for now

    int x, y;
    for(x = 0; x < width; x++)
    {
        for(y = 0; y < height; y++)
        {
            int index = x * height + y;
            this->tiles[index].set_id(0);
            this->tiles[index].set_angle(0);
            this->tiles[index].set_tile_type(tile::TILE_TYPE_PASSABLE); //able to be walked through
        }
    }

    this->tileset_path = QString("textures.png");
    char buffer[32];
    sprintf(buffer, "res/%s", this->tileset_path.toStdString().c_str());
    //TODO: if allocate texture statement?

    this->tileset_texture = new QOpenGLTexture(QImage(QString(buffer)));

    this->map_name = "Test Map";
}
