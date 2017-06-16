#ifndef VECTOR_H
#define VECTOR_H


class vector
{
public:
    vector();
    vector(float _x, float _y);
    float magnitude(vector v);
    vector to_opengl_coordinates();
    float x, y;
};

#endif // VECTOR_H
