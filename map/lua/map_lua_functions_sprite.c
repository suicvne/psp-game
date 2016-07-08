#include "map_lua_functions.h"

int lua_sprite_create(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count > 0)
    {
        const char* path = lua_tostring(L, 1);

        sprite_t* sprite = sprite_create(path, SPRITE_TYPE_PNG);
        if(sprite != NULL)
        {
            lua_pushlightuserdata(L, sprite);
            return 1;
        }
    }

    return 0;
}

int lua_sprite_destroy(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 1)
    {
        sprite_t* sprite = ((sprite_t*)lua_touserdata(L, 1));
        if(sprite != NULL)
        {
            sprite_destroy(sprite);
            return 0;
        }
    }

    return 0;
}

int lua_sprite_set_position(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 3) //sprite, x, y
    {
        sprite_t* sprite;
        sprite = ((sprite_t*)lua_touserdata(L, 1));

        int x, y;
        x = lua_tonumber(L, 2);
        y = lua_tonumber(L, 3);

        if(sprite != NULL)
        {
            sprite->rectangle.x = x;
            sprite->rectangle.y = y;
            return 0;
        }
    }
    return 0;
}

int lua_sprite_set_size(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 3) //sprite, w, h
    {
        sprite_t* sprite;
        sprite = ((sprite_t*)lua_touserdata(L, 1));

        int width, height;
        width = lua_tonumber(L, 2);
        height = lua_tonumber(L, 3);

        if(sprite != NULL)
        {
            sprite->rectangle.w = width;
            sprite->rectangle.h = height;
            return 0;
        }
    }
    return 0;
}

int lua_sprite_set_frame_count(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 2) //sprite, frame count
    {
        sprite_t* sprite;
        sprite = ((sprite_t*)lua_touserdata(L, 1));

        int framecount;
        framecount = lua_tonumber(L, 2);

        if(sprite != NULL)
        {
            sprite->frames = framecount;
            return 0;
        }
    }
    return 0;
}

int lua_sprite_set_frame_time(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 2) //sprite, frame count
    {
        sprite_t* sprite;
        sprite = ((sprite_t*)lua_touserdata(L, 1));

        int frametime;
        frametime = lua_tonumber(L, 2);

        if(sprite != NULL)
        {
            sprite->frametime = frametime;
            return 0;
        }
    }
    return 0;
}

int lua_sprite_update(lua_State* L)
{
    int arg_count = lua_gettop(L);
    if(arg_count == 1)
    {
        sprite_t* sprite;
        sprite = ((sprite_t*)lua_touserdata(L, 1));
        if(sprite != NULL)
        {
            sprite_update(sprite);
            return 0;
        }
    }

    return 0;
}