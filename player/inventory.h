/**

Inventory is tied to the player class!!!!!! Do not initialize this anywhere else!!

*/

#ifndef ___INVENTORY_H___
#define ___INVENTORY_H___

#include <stdlib.h>
#include <assert.h>

#define BLANK 0
#define GRASS_SEED 1

/**

All items will be contained on a single sprite sheet.

Items will be divided into two types (for now):
 - Usable tools
 - Items

All the C interface will handle is storing the index of the item on the sheet. Lua will handle handling of items.

*/

extern int* player_inventory;
extern const int inventory_max_items;
extern const int inventory_item_sheet_width;
extern const int inventory_item_sheet_height;
extern const int inventory_item_width;
extern const int inventory_item_height;

void inventory_create();
void inventory_destroy();

void inventory_push_item(int item);
void inventory_pop_item(int index);
int inventory_get_item(int index);

void inventory_get_item_sheet_location(int item, int* x, int* y);


#endif //___INVENTORY_H___