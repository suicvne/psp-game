#ifndef TILEMAP_H
#define TILEMAP_H

#include "tile.h"
#include <QString>
#include <QOpenGLTexture>

#define HEADER_0 'M'
#define HEADER_1 'S'
#define VERSION 04

class tilemap
{
public:
    tilemap(int _width, int _height);

private:
    //Tiles are STILL stored in a single dimensional array.
    //You can access objects by doing
    //tiles[x * h + y]
    tile* tiles;
    int width, height;
    QString map_name;
    short surrounding_tile_id;
    QString tileset_path;
    QOpenGLTexture* tileset_texture;
};

#endif // TILEMAP_H
