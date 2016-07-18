#include "inventory.h"

int* player_inventory;
INVENTORY_MODE inventory_display_mode = INVENTORY_MODE_HOTBAR;
int inventory_hotbar_index = 0;

const int inventory_hotbar_size = 8; //even number. first 8 slots are for hotbar, remaining 24 (3 more rows) for misc. items

const int inventory_max_items = 32;
const int inventory_item_sheet_width = 256;
const int inventory_item_sheet_height = 256;
const int inventory_item_ui_sheet_width = 128;
const int inventory_item_ui_sheet_height = 128;
const int inventory_item_width = 32;
const int inventory_item_height = 32;

int inventory_starting_x = 110;
int inventory_starting_y = 4;

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
    memset(player_inventory, BLANK, 32);

    player_inventory[0] = 0;
    player_inventory[1] = GRASS_SEED;
    //player_inventory[2] = 2;
    //player_inventory[3] = 3;
    //player_inventory[4] = 4;
    //player_inventory[5] = 5;
    //player_inventory[6] = 6;
    //player_inventory[7] = 7;
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

void inventory_draw()
{
    if(inventory_display_mode == INVENTORY_MODE_HOTBAR)
    {
        int i;
        for(i = 0; i < 8; i++)
        {
            int x = (inventory_starting_x + (i * 34));
            sprite_draw_source(kInventory, x, inventory_starting_y, 0, 0, 32, 32);
            if(inventory_hotbar_index == i)
            {
                sprite_draw_source(kInventory, x, inventory_starting_y, 32, 0, 32, 32);
            }
            
            int item_x, item_y;
            inventory_get_item_sheet_location(player_inventory[i], &item_x, &item_y);
            sprite_draw_source(kItems, x, inventory_starting_y, item_x, item_y, 32, 32);
        }
    }
    else if(inventory_display_mode == INVENTORY_MODE_FULLSCREEN)
    {
        inventory_draw_full();
    }
}

void inventory_draw_full()
{
    //TODO: draw full inventory here
}

void inventory_update()
{
    //TODO: this will be for handling input and whatnot
}