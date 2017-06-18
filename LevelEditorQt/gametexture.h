#ifndef GAMETEXTURE_H
#define GAMETEXTURE_H

#include <QOpenGLTexture>
#include <QImage>

class gametexture
{
public:
    gametexture(QImage image);
    ~gametexture();

    QOpenGLTexture* toOpenGLTexture();
    QImage toQImage();
private:
    QImage qImageTexture;
    QOpenGLTexture *oglTexture;
};

#endif // GAMETEXTURE_H
