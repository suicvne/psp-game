#include "customopenglwidget.h"

#include <QtOpenGL>

#include <iostream>
#include <math.h>

#include "mainwindow.h"
#include <rocklevel/camera.h>
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

    main_camera = camera_create(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc (GL_ONE, GL_ONE);
    glBlendFunc(GL_ONE, GL_ONE);

    this->main_texture = loadTexture(MainWindow::getResourcesDirectory() + "/textures.png");
    if(this->main_texture == NULL)
        qApp->exit(0);
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->drawTilemap();

    vector_t mouse = mouseToGame();
    float tx = (floor(mouse.x / 32) * 32);
    float ty = (floor(mouse.y / 32) * 32);
    drawRectangle(tx, ty, TILE_WIDTH, TILE_HEIGHT, placingTileRotation, placingTileID);
}

int CustomOpenGLWidget::getPlacingTileRotation()
{
    return placingTileRotation;
}

void CustomOpenGLWidget::setPlacingTileRotation(int rotation)
{
    if(rotation > 360)
        placingTileRotation = 0;
    else if(rotation < 0)
        placingTileRotation = 360;
    else
        placingTileRotation = rotation;
}

void CustomOpenGLWidget::placeTileAction(bool isSolid)
{
    vector_t mouse = mouseToGame();
    int tx, ty;
    tx = floor((mouse.x) / 32) + -floor(main_camera->x / 32); //+ floor(main_camera->x / 32);
    ty = floor((mouse.y) / 32) + -floor(main_camera->y / 32); //+ floor(main_camera->y / 32);

    if(tx < 0 || tx > currentTilemap->width)
        return;
    if(ty < 0 || ty > currentTilemap->height)
        return;

    std::cout <<  "placing at " << tx << ", " << ty << std::endl;

    int index = tx * currentTilemap->height + ty;
    currentTilemap->tiles[index].id = placingTileID;
    currentTilemap->tiles[index].angle = placingTileRotation;
    if(isSolid)
        currentTilemap->tiles[index].tile_type = TILE_TYPE_SOLID;
}

void CustomOpenGLWidget::moveCamera(float _x, float _y)
{
    vector_t adjustment { _x, _y };
    camera_move(main_camera, adjustment);
}

void CustomOpenGLWidget::setCameraPosition(float x, float y)
{
    main_camera->x = x == -1 ? main_camera->x : x;
    main_camera->y = y == -1 ? main_camera->y : y; //shouldn't really do this but oh well ¯\_(ツ)_/¯
}

void CustomOpenGLWidget::setTileMapName(QString name)
{
    std::cout << "map name to " << name.toStdString() << std::endl;

    char* cstr;
    std::string stdName = name.toStdString();
    cstr = new char [stdName.size()+1];
    strcpy( cstr, stdName.c_str() );

    this->currentTilemap->map_name = cstr;

    std::cout << "map name: " << this->currentTilemap->map_name << std::endl;;
}

gametexture* CustomOpenGLWidget::loadTexture(QString path)
{
    QImage* texture = new QImage(path);
    if(!texture->isNull())
    {
        return new gametexture(texture);
    }
    else
    {
        QMessageBox::critical(this, "Editor", "Error loading texture.\nThe texture couldn't be loaded from the path " + path
                              + "\n\nOpening resources directory now!",
                              QMessageBox::Ok, QMessageBox::Ok);
        MainWindow::OpenResourcesDirectory();

        qApp->quit();
    }

    return NULL;
}

void CustomOpenGLWidget::loadTestTextures()
{
    //QOpenGLTexture()
    //TODO: this and change return type!
    QString testTexturePath = qApp->applicationDirPath() + "/res/textures.png";

    //QImage textureAtlas(testTexturePath);
    QImage* textureAtlas = new QImage(testTexturePath);
    if(!textureAtlas->isNull())
        this->main_texture = new gametexture(textureAtlas);
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
            drawRectangle((x * TILE_WIDTH) + main_camera->x,
                          (y * TILE_WIDTH) + main_camera->y,
                          TILE_HEIGHT, TILE_WIDTH, tile.angle, tile.id
            );

            if(this->drawCollisionMap)
            {
                if(tile.tile_type == TILE_TYPE_PASSABLE)
                {
                    drawColoredRectangle((x * TILE_WIDTH) + main_camera->x,
                                         (y * TILE_WIDTH) + main_camera->y,
                                         TILE_WIDTH, TILE_HEIGHT, 0.0f, 0.0f, .34f, 0.2f
                                         );
                }
                else if(tile.tile_type == TILE_TYPE_SOLID)
                {
                    drawColoredRectangle((x * TILE_WIDTH) + main_camera->x,
                                         (y * TILE_WIDTH) + main_camera->y,
                                         TILE_WIDTH, TILE_HEIGHT, 1.0f, 0, 0, 0.2f
                                         );
                }
            }
        }

    }
}

void CustomOpenGLWidget::setDrawCollisionMap(bool draw)
{
    this->drawCollisionMap = draw;
}

bool CustomOpenGLWidget::getDrawCollisionMap()
{
    return this->drawCollisionMap;
}

void CustomOpenGLWidget::drawColoredRectangle(float x, float y, float w, float h, float r, float g, float b, float a)
{
    //glShadeModel(GL_SMOOTH);
    glBegin(GL_QUADS);


    glColor4f(r, g, b, a);
    glVertex2f(x, y);

    glColor4f(r, g, b, a);
    glVertex2f(x + w, y);

    glColor4f(r, g, b, a);
    glVertex2f(x + w, y + h);

    glColor4f(r, g, b, a);
    glVertex2f(x, y + h);

    glEnd();

    //glBlendFunc(GL_NONE, GL_NONE);
    //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
    glColor4f(1, 1, 1, 1);
}

void CustomOpenGLWidget::drawRectangle(float x, float y, float w, float h, int rotation, int sheet_id)
{
    if(main_texture == NULL) return;

    //vector_t tile_location = this->returnTileAreaByID(sheet_id);
    vector_t tile_location = tile_get_location_by_id(sheet_id);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, main_texture->toOpenGLTexture()->textureId());

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
    glDisable(GL_TEXTURE_2D);

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
    glBindTexture(GL_TEXTURE_2D, main_texture->toOpenGLTexture()->textureId());
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

    return value;
}

gametexture* CustomOpenGLWidget::getMainTexture()
{
    return main_texture;
}

bool CustomOpenGLWidget::loadTilemap(QString file)
{
    tilemap_t* loaded = tilemap_read_from_file(file.toStdString().c_str());

    if(loaded != NULL)
    {
        this->currentTilemap = loaded;
        //delete this->main_texture;
        this->main_texture = loadTexture(MainWindow::getResourcesDirectory() + "/" + QString(this->currentTilemap->tileset_path));
        if(this->main_texture == NULL) //load failed
        {
            std::cout << "load failed of texture" << std::endl;
            return false; //cancel the load blowing
        }

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

void CustomOpenGLWidget::newTilemap(QString levelName, int width, int height, QString tilesetPath, QString tilesetPath2)
{
    tilemap_t* new_tilemap = tilemap_create(width, height, 0);
    new_tilemap->tileset_path = (char*)tilesetPath.toStdString().c_str();
    new_tilemap->map_name = (char*)levelName.toStdString().c_str();

    //TODO: layer 2
    if(new_tilemap != NULL)
    {
        tilemap_destroy(this->currentTilemap);
        this->currentTilemap = new_tilemap;
        this->main_texture = loadTexture(MainWindow::getResourcesDirectory() + "/" + QString(this->currentTilemap->tileset_path));
    }
}

int CustomOpenGLWidget::getPlacingTileID()
{
    return placingTileID;
}

void CustomOpenGLWidget::setPlacingTileID(int id)
{
    this->placingTileID = id;
}
