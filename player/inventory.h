/**

Inventory is tied to the player class!!!!!! Do not initialize this anywhere else!!

*/

#ifndef ___INVENTORY_H___
#define ___INVENTORY_H___

#include <stdlib.h>
#include <assert.h>

#include "../sprites/sprite.h"
#include "../input/input.h"
#include "../globals.h"

#define BLANK 0
#define GRASS_SEED 1
#define TEST_HOE 2

typedef enum {

    INVENTORY_MODE_HOTBAR = 0, //hotbar only
    INVENTORY_MODE_FULLSCREEN = 1 //Full inventory being drawn

} INVENTORY_MODE;

/**

All items will be contained on a single sprite sheet.

Items will be divided into two types (for now):
 - Usable tools
 - Items

All the C interface will handle is storing the index of the item on the sheet. Lua will handle handling of items.

*/

extern int* player_inventory;
extern INVENTORY_MODE inventory_display_mode;
extern int inventory_hotbar_index;

extern const int inventory_max_items;
extern const int inventory_hotbar_size;
extern const int inventory_item_sheet_width;
extern const int inventory_item_sheet_height;
extern const int inventory_item_ui_sheet_width;
extern const int inventory_item_ui_sheet_height;
extern const int inventory_item_width;
extern const int inventory_item_height;

void inventory_create();
void inventory_destroy();

void inventory_push_item(int item);
void inventory_pop_item(int index);
int inventory_get_item(int index);

void inventory_draw(); //draws the hotbar on screen
void inventory_draw_full(); //draws the full inventory on screen

void inventory_update();

vector_t inventory_get_item_sheet_location(int item);


#endif //___INVENTORY_H___