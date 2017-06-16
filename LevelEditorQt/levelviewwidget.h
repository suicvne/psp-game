/**

  This is where the actual drawing is done for the level.

  */

#ifndef LEVELVIEWWIDGET_H
#define LEVELVIEWWIDGET_H
#include <QtOpenGL>
#include <QWidget>
#include <QOpenGLWidget>
#include <gl.h>
#include <QMatrix4x4>
#include <QOpenGLFunctions>

#include "tilemap.h"


class LevelViewWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    LevelViewWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void loadTestTextures();
    void drawRectangle(float x, float y, float w, float h);
    void draw_tile(tile* t);

    void draw_tilemap();

private:
    QMatrix4x4 m_projection;
    QOpenGLTexture* texture_atlas;
    tilemap* map;
};

#endif // LEVELVIEWWIDGET_H
