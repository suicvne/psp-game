/**

  This is where the actual drawing is done for the level.

  */

#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

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
    bool loadTilemap(QString file);

    void setTileMapName(QString name);

private:
    void drawTilemap();

    vector_t mouseToGame();

    void rotate0(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate90(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate180(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);
    void rotate270(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset);

    QOpenGLTexture* loadTexture(QString path);

    //QMatrix4x4 m_projection;
    QOpenGLTexture* main_texture;
    tilemap_t* currentTilemap;
};

#endif // CUSTOMOPENGLWIDGET_H
