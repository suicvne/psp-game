#include "map_lua_functions.h"

int lua_message_box_is_visible(lua_State* L)
{
    lua_pushboolean(L, message_box_is_visible);
    return 1;
}

int lua_message_box_set_visible(lua_State* L)
{
    if(lua_gettop(L) == 1)
    {
        int visiblility = lua_toboolean(L, 1);
        message_box_set_visibility(visiblility);
    }
    return 0;
}

int lua_message_box_set_message(lua_State* L)
{
    if(lua_gettop(L) == 1)
    {
        const char* message = lua_tostring(L, 1);
        message_box_set_message(message);
    }
    return 0;
}

int lua_message_box_get_message(lua_State* L)
{
    if(message_box_message != NULL)
    {
        lua_pushstring(L, message_box_message);
        return 1;
    }
    return 0;
}