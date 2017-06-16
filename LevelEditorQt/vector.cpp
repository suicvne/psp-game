#include "vector.h"

#include <cmath>
#include "tile.h"

vector::vector()
{
    x = 0.0f;
    y = 0.0f;
}

/**
 * @brief tile::to_opengl_coordinates Converts regular tile coordinates into OpenGL coordinates for reference.
 * @param v
 * @return
 */
vector vector::to_opengl_coordinates()
{
    vector return_value(this->x, this->y);

    return_value.x = (this->x / SHEET_WIDTH);
    return_value.y = (this->y / SHEET_HEIGHT);

    return return_value;
}

vector::vector(float _x, float _y):x(_x), y(_y)
{/*i can't believe i actually remembered how to write that^*/}

float vector::magnitude(vector v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}
