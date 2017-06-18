#include "gametexture.h"

gametexture::gametexture(QImage* image) : qImageTexture(image)
{
    this->oglTexture = new QOpenGLTexture(*image);
}

gametexture::~gametexture()
{
    oglTexture->destroy();
    delete qImageTexture;
    delete oglTexture;
}

QImage* gametexture::toQImage()
{
    return qImageTexture;
}

QOpenGLTexture* gametexture::toOpenGLTexture()
{
    return oglTexture;
}
