#ifndef ___GRAPHICS_COMMON_H___
#define ___GRAPHICS_COMMON_H___

static inline void graphics_get_sheet_location_by_id(int id, int sheet_width, int sheet_height, int item_width, 
                                                     int item_height, int* x, int* y)
{
    int max_tiles = (sheet_width * sheet_height) / (item_width * item_height);
    if(id > max_tiles)
    {
        (*x) = 0;
        (*y) = 0;
        return;
    }

    (*x) = id * item_width;
    (*y) = 0;
    while((*x) >= sheet_width)
    {
        (*x) -= sheet_width;
        (*y) += item_height;
    }
}

#endif //___GRAPHICS_COMMON_H___
