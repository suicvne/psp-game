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

vector_t inventory_get_item_sheet_location(int index)
{
    assert(index < inventory_max_items);
    int id = player_inventory[index];

    vector_t return_value;

    int max_tiles = (inventory_item_sheet_width * inventory_item_sheet_height) / (inventory_item_width * inventory_item_height);
    if(id > max_tiles)
    {
        return_value.x = 0;
        return_value.y = 0;
        return;
    }

    return_value.x = id * inventory_item_width;
    return_value.y = 0;
    while(return_value.x >= inventory_item_sheet_width)
    {
        return_value.x -= inventory_item_sheet_width;
        return_value.y += inventory_item_height;
    }

    return return_value;
}

void inventory_create()
{
    player_inventory = malloc(sizeof(int) * inventory_max_items);
    memset(player_inventory, BLANK, 32);

    player_inventory[0] = TEST_HOE; //2
    player_inventory[1] = GRASS_SEED; //1
    //player_inventory[2] = 2;
    //player_inventory[3] = 3;
    //player_inventory[4] = 4;
    //player_inventory[5] = 5;
    //player_inventory[6] = 6;
    //player_inventory[7] = 7;
    int i ;
    for(i = 0; i < 8; i++)
    {
        printf("inv at %d: %d\n", i, player_inventory[i]);
    }
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
            
            vector_t item_location = inventory_get_item_sheet_location(i);
            
            sprite_draw_source(kItems, x, inventory_starting_y, item_location.x, item_location.y, 32, 32);
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
    if(input_is_button_just_pressed(INPUT_BUTTON_HOTBAR_NEXT))
    {
        inventory_hotbar_index++;
        if(inventory_hotbar_index > 7) //TODO: make this a constant
            inventory_hotbar_index = 0;
    }
    else if(input_is_button_just_pressed(INPUT_BUTTON_HOTBAR_BACK))
    {
        inventory_hotbar_index--;
        if(inventory_hotbar_index < 0)
            inventory_hotbar_index = 7; //TODO: make this a constant
    }
}