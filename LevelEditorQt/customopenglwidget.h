/**

  This is where the actual drawing is done for the level.

  */

#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "gametexture.h"
#include <rocklevel/tilemap.h>
#include <rocklevel/vector.h>
//#include "tile.h"



class CustomOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    CustomOpenGLWidget(QWidget *parent = 0);
    void initializeGL();
    //void resizeGL(int w, int h);
    void paintGL();

    void loadTestTextures();
    void drawRectangle(float x, float y, float w, float h);
    void drawRectangle(float x, float y, float w, float h, int rotation, int sheet_id);
    vector_t returnTileAreaByID(int id);

    tilemap_t* getCurrentTilemap();
    gametexture* getMainTexture();
    bool loadTilemap(QString file);

    void setTileMapName(QString name);

    int getPlacingTileID();
    void setPlacingTileID(int id);

    int getPlacingTileRotation();
    void setPlacingTileRotation(int rotation);

    void placeTileAction();

private:
    void drawTilemap();

    vector_t mouseToGame();

    void rotate0(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate90(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate180(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate270(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);

    gametexture* loadTexture(QString path);

    //QMatrix4x4 m_projection;
    //QOpenGLTexture* main_texture;
    gametexture* main_texture;
    tilemap_t* currentTilemap;

    int placingTileID = 0; //the ID of the tile we'll be placing
    int placingTileRotation = 0; //the rotation of the tile we'll be placing
};

#endif // CUSTOMOPENGLWIDGET_H
