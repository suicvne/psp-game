#include "inventory.h"

int* player_inventory;
const int inventory_max_items = 32;
const int inventory_item_sheet_width = 256;
const int inventory_item_sheet_height = 256;
const int inventory_item_width = 32;
const int inventory_item_height = 32;

void inventory_get_item_sheet_location(int index, int* x, int* y)
{
    assert(index < inventory_max_items);
    int id = player_inventory[index];

    int max_tiles = (inventory_item_sheet_width * inventory_item_sheet_height) / (inventory_item_width * inventory_item_height);
    if(id > max_tiles)
    {
        (*x) = 0;
        (*y) = 0;
        return;
    }

    (*x) = id * inventory_item_width;
    (*y) = 0;
    while((*x) >= inventory_item_sheet_width)
    {
        (*x) -= inventory_item_sheet_width;
        (*y) += inventory_item_height;
    }
}

void inventory_create()
{
    player_inventory = malloc(sizeof(int) * inventory_max_items);
    memset(player_inventory, 0, 32);

    player_inventory[0] = GRASS_SEED;
}

void inventory_destroy()
{
    free(player_inventory);
}

void inventory_push_item(int item)
{
    int i;
    for(i = 0; i < inventory_max_items; i++)
    {
        if(player_inventory[i] == 0) //free space in inventory
        {
            player_inventory[i] = item;
        }
    }
}

void inventory_pop_item(int index)
{
    assert(index < inventory_max_items);
    player_inventory[index] = BLANK;
}

int inventory_get_item(int index)
{
    assert(index < inventory_max_items);
    return player_inventory[index];
}