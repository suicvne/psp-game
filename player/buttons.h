#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "../vector/vector.h"
#include "../globals.h"
#include "../sprite/sprite.h"

#define BUTTON_SHEET_WIDTH 128
#define BUTTON_SHEET_HEIGHT 128
#define BUTTON_SIZE 16

typedef enum {
	
	X_button = 0,
	tri_button,
	O_button,
	sqr_button,
	start_button,
	select_button,
	analogue_button,
	dpad_up_button,
	dpad_left_button,
	dpad_down_button,
	dpad_right_button
	
} button_t;

static inline vector_t button_get_button_by_id(button_t button_id)
{
	return sprite_get_location_by_index(kButtons, BUTTON_SIZE, ((int)button_id));
}

#endif //__BUTTONS_H__