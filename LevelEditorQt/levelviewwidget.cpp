#include "levelviewwidget.h"

LevelViewWidget::LevelViewWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);

    map = new tilemap(128, 128);
}

void LevelViewWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    loadTestTextures();
}

void LevelViewWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, w / float(h), 1, 1000.0f);
}

void LevelViewWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //texture_atlas->bind();
    drawRectangle(32, 32, 128, 128);
}

void LevelViewWidget::loadTestTextures()
{
    //QOpenGLTexture()
    //TODO: this and change return type!
    this->texture_atlas = new QOpenGLTexture(QImage("res/textures.png"));

}

void LevelViewWidget::draw_tilemap()
{

}

void LevelViewWidget::draw_tile(tile* t)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_atlas->textureId());
    glBegin(GL_QUADS);

    vector tile_location = t->get_location_by_id();

    //TODO
}

void LevelViewWidget::drawRectangle(float x, float y, float w, float h)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_atlas->textureId());
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);
    glVertex2f(x, y); //Top left

    glTexCoord2f(1.0, 0.0);
    glVertex2f(x + w, y); //Top right

    glTexCoord2f(1.0, 1.0);
    glVertex2f(x + w, y + h); //Bottom right

    glTexCoord2f(0.0, 1.0);
    glVertex2f(x, y + h); //Bottom left
    glEnd();
    glBindTexture(0, 0);
}
