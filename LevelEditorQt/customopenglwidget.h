/**

  This is where the actual drawing is done for the level.

  */

#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QtOpenGL>

#ifdef Q_OS_WIN
#include <GL/gl.h>
#endif

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "gametexture.h"
#include <rocklevel/tilemap.h>
#include <rocklevel/vector.h>
#include <rocklevel/camera.h>
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
    void drawColoredRectangle(float x, float y, float w, float h, float r, float g, float b, float a);
    void drawRectangle(float x, float y, float w, float h, int rotation, int sheet_id);
    vector_t returnTileAreaByID(int id);

    tilemap_t* getCurrentTilemap();
    gametexture* getMainTexture();
    bool loadTilemap(QString file);

    void newTilemap(QString levelName, int width, int height, QString tilesetPath, QString tilesetPath2 = "");

    void setTileMapName(QString name);

    int getPlacingTileID();
    void setPlacingTileID(int id);

    int getPlacingTileRotation();
    void setPlacingTileRotation(int rotation);

    void placeTileAction(bool isSolid = false);

    void moveCamera(float _x, float _y);
    void setCameraPosition(float x, float y);

    void setDrawCollisionMap(bool draw);
    bool getDrawCollisionMap();

private:
    void drawTilemap();

    vector_t mouseToGame();

    void rotate0(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate90(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate180(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate270(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);


    camera_t* main_camera;
    gametexture* loadTexture(QString path);

    //QMatrix4x4 m_projection;
    //QOpenGLTexture* main_texture;
    gametexture* main_texture;
    tilemap_t* currentTilemap;

    bool drawCollisionMap = false; //don't draw the collision map by default.

    int placingTileID = 0; //the ID of the tile we'll be placing
    int placingTileRotation = 0; //the rotation of the tile we'll be placing
};

#endif // CUSTOMOPENGLWIDGET_H
