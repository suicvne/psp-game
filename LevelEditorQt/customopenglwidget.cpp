#include "customopenglwidget.h"

#include <QtOpenGL>

#include <iostream>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);

    //prepare placeholder tilemap
    currentTilemap = tilemap_create(32, 32, 0);
    this->main_texture = NULL; //lol this is probably a terrible idea
    //end prepare placeholder tilemap
}

void CustomOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 480, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    this->main_texture = loadTexture("res/textures.png");
}

/*
void CustomOpenGLWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, w / float(h), 1, 1000.0f);
}
*/

void CustomOpenGLWidget::paintGL()
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    this->drawTilemap();
    /*
    //texture_atlas->bind();
    drawRectangle(32, 32, TILE_WIDTH, TILE_HEIGHT, 0);
    drawRectangle(64 + 32, 32, TILE_WIDTH, TILE_HEIGHT, 2);
    drawRectangle(64 + 64, 32, TILE_WIDTH, TILE_HEIGHT, 8);
    */

    //drawRectangle(32, 32, 256, 256);

    //renders the mouse

    //float mx, my;
    //mx = floor(this->mapFromGlobal(QCursor::pos()).x() / TILE_WIDTH);
    //my = floor(this->mapFromGlobal(QCursor::pos()).y() / TILE_WIDTH);
    vector_t mouse = mouseToGame();
    drawRectangle(floor(mouse.x / 32) * 32, floor(mouse.y / 32) * 32, TILE_WIDTH, TILE_HEIGHT, 0, 4);

}

void CustomOpenGLWidget::setTileMapName(QString name)
{
    this->currentTilemap->map_name = (char*)name.toStdString().c_str();
}

QOpenGLTexture* CustomOpenGLWidget::loadTexture(QString path)
{
    QImage texture(path);
    if(!texture.isNull())
    {
        return new QOpenGLTexture(texture);
    }
    else
    {
        QMessageBox::critical(this, "Editor", "Error loading texture.\nThe texture couldn't be loaded from the path " + path,
                              QMessageBox::Ok, QMessageBox::Ok);
        /*
        QMessageBox box;
        box.setText("Error loading texture");
        box.setInformativeText("Couldn't load the texture you requested given the path " + path);
#if __APPLE__
        //box.setModal(true);
#endif
        box.exec();
        */
    }

    return NULL;
}

void CustomOpenGLWidget::loadTestTextures()
{
    //QOpenGLTexture()
    //TODO: this and change return type!
    QString testTexturePath = qApp->applicationDirPath() + "/res/textures.png";

    QImage textureAtlas(testTexturePath);
    if(!textureAtlas.isNull())
        this->main_texture = new QOpenGLTexture(textureAtlas);
    else
    {
        QMessageBox box(this);
        box.setText("Error loading texture atlas from " + testTexturePath);
        box.exec();

        qApp->quit();
    }

    //this->texture_atlas->setAutoMipMapGenerationEnabled(false);
    //this->texture_atlas->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
}

void CustomOpenGLWidget::drawTilemap()
{
    for(int x = 0; x < currentTilemap->width; x++)
    {
        for(int y = 0; y < currentTilemap->height; y++)
        {
            int index = x * currentTilemap->height + y;
            tile_t tile = currentTilemap->tiles[index];
            drawRectangle(x * TILE_WIDTH, y * TILE_WIDTH, TILE_HEIGHT, TILE_WIDTH, tile.angle, tile.id);

            //TODO: camera, angles. not camera angles, this is a 2D game
        }
    }
}

void CustomOpenGLWidget::drawRectangle(float x, float y, float w, float h, int rotation, int sheet_id)
{
    if(main_texture == NULL) return;

    vector_t tile_location = this->returnTileAreaByID(sheet_id);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, main_texture->textureId());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //disables filtering for scaling down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //disables filtering for scaling up


    glBegin(GL_QUADS);

    float topLeftX = (tile_location.x / (float)SHEET_WIDTH);
    float topLeftY = (tile_location.y / (float)SHEET_HEIGHT);
    float offset = ((float)TILE_WIDTH / (float)SHEET_WIDTH);

    switch(rotation)
    {
    case 90:
        rotate90(x, y, w, h, topLeftX, topLeftY, offset);
        break;
    case 180:
        rotate180(x, y, w, h, topLeftX, topLeftY, offset);
        break;
    case 270:
        rotate270(x, y, w, h, topLeftX, topLeftY, offset);
        break;
    default:
        rotate0(x, y, w, h, topLeftX, topLeftY, offset);
        break;
    }
    glEnd();

    glBindTexture(0, 0);
}

void CustomOpenGLWidget::rotate0(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset)
{
    glTexCoord2f(topLeftX, topLeftY); //Top left
    glVertex2f(x, y); //Top left

    glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glVertex2f(x + w, y); //Top right

    glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glVertex2f(x + w, y + h); //Bottom right

    glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glVertex2f(x, y + h); //Bottom left
}

void CustomOpenGLWidget::rotate90(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset)
{
    //glTexCoord2f(topLeftX, topLeftY); //Top left
    glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glVertex2f(x, y); //Top left

    //glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glTexCoord2f(topLeftX, topLeftY); //Top left
    glVertex2f(x + w, y); //Top right

    //glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glVertex2f(x + w, y + h); //Bottom right

    //glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glVertex2f(x, y + h); //Bottom left
}

void CustomOpenGLWidget::rotate180(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset)
{
    //glTexCoord2f(topLeftX, topLeftY); //Top left
    glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glVertex2f(x, y); //Top left

    //glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glVertex2f(x + w, y); //Top right

    //glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glTexCoord2f(topLeftX, topLeftY); //Top left
    glVertex2f(x + w, y + h); //Bottom right

    //glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glVertex2f(x, y + h); //Bottom left
}

void CustomOpenGLWidget::rotate270(float x, float y, float w, float h, float topLeftX, float topLeftY, float offset)
{
    //glTexCoord2f(topLeftX, topLeftY); //Top left
    glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glVertex2f(x, y); //Top left

    //glTexCoord2f(topLeftX + offset, topLeftY); //Top right
    glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glVertex2f(x + w, y); //Top right

    //glTexCoord2f(topLeftX + offset, topLeftY + offset); //Bottom right
    glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glVertex2f(x + w, y + h); //Bottom right

    //glTexCoord2f(topLeftX, topLeftY + offset); //Bottom left
    glTexCoord2f(topLeftX, topLeftY); //Top left
    glVertex2f(x, y + h); //Bottom left
}

vector_t CustomOpenGLWidget::returnTileAreaByID(int id)
{
    /*
    int SHEET_WIDTH = 256;
    int SHEET_HEIGHT = 256;
    int TILE_WIDTH = 32;
    int TILE_HEIGHT = 32;
    */

    vector_t return_value;
    int max_tiles = (SHEET_WIDTH * SHEET_HEIGHT) / (TILE_WIDTH * TILE_HEIGHT);
    if(id > max_tiles)
    {
      return return_value; //should default to 0, 0
    }

    return_value.x = id * TILE_WIDTH;
    return_value.y = 0;
    while(return_value.x >= SHEET_WIDTH)
    {
      return_value.x -= SHEET_WIDTH;
      return_value.y += TILE_HEIGHT;
    }
    return return_value;
}

void CustomOpenGLWidget::drawRectangle(float x, float y, float w, float h)
{
    if(main_texture == NULL) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, main_texture->textureId());
    //disables filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

tilemap_t* CustomOpenGLWidget::getCurrentTilemap()
{
    return currentTilemap;
}

vector_t CustomOpenGLWidget::mouseToGame()
{
    float widget_width, widget_height;
    widget_width = this->size().width();
    widget_height = this->size().height();

    float w_scale;
    float h_scale;

    w_scale = widget_width / (float)SCREEN_WIDTH;
    h_scale = widget_height / (float)SCREEN_HEIGHT;

    vector_t value;
    value.x = this->mapFromGlobal(QCursor::pos()).x() / w_scale;
    value.y = this->mapFromGlobal(QCursor::pos()).y() / h_scale;
    /*
    if(camera != NULL)
    {
      value.x = ((int)-camera->x) + (input_current_frame.mouse_x) / w_scale;
      value.y = ((int)-camera->y) + (input_current_frame.mouse_y) / h_scale;
    }
    else
    {
      value.x = (input_current_frame.mouse_x) / w_scale;
      value.y = (input_current_frame.mouse_y) / h_scale;
    }
    */

    return value;
}

bool CustomOpenGLWidget::loadTilemap(QString file)
{
    tilemap_t* loaded = tilemap_read_from_file(file.toStdString().c_str());

    if(loaded != NULL)
    {
        this->currentTilemap = loaded;
        QOpenGLTexture* theTexture = loadTexture("res/" + QString(this->currentTilemap->tileset_path));
        if(theTexture == NULL) //load failed
            return false; //cancel the load blowing
        this->main_texture = loadTexture("res/" + QString(this->currentTilemap->tileset_path));
        return true;
    }
    else
    {
        QMessageBox box(NULL);
        box.setText("Error loading tilemap from " + file);
        box.exec();
    }

    return false;
}
