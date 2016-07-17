#include "map_lua_functions.h"

int lua_inventory_get_inventory(lua_State* L)
{
    if(player_inventory != NULL)
    {
        //lua_createtable(L, 0, inventory_max_items);
        lua_newtable(L);

        int i;
        for(i = 0; i < inventory_max_items; i++)
        {
            lua_pushnumber(L, i);
            lua_pushnumber(L, inventory_get_item(i));
            lua_settable(L, -3);
        }

        //implied that it is top of the stack
        return 1;
    }

    return 0;
} //returns inventory as table

/*
int lua_inventory_push_item(lua_State* L)
{

}

int lua_inventory_pop_item(lua_State* L)
{

}

int lua_inventory_get_item(lua_State* L)
{

}
*/