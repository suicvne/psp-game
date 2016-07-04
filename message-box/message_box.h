/**

Contains various functions for in-game message boxes.

*/

#ifndef ___MESSAGE_BOX_H___
#define ___MESSAGE_BOX_H___

#include <stdio.h>
#include <string.h>

#include "../graphics/rectangle.h"
#include "../graphics/text.h"

extern const int MESSAGE_BOX_MAX_CHAR_LENGTH; //maximum characters per line for a message
extern const int MESSAGE_BOX_MAX_LINES; //maximum lines per single message box
extern const int MESSAGE_BOX_MAX_WIDTH; //maximum width in pixels that the message box should be.
extern const int MESSAGE_BOX_MAX_HEIGHT; //maximum height in pixels that the message box should be.

extern const int MESSAGE_BOX_ORIGIN_X; //where the message box is to start drawing
extern const int MESSAGE_BOX_ORIGIN_Y; //where the message box is to start drawing

extern int message_box_framecount;
extern int message_box_is_visible;

extern rectangle_t message_box_rectangle;

extern char* message_box_message;

void message_box_set_message(const char* message); //copies the specified message into the current message variable 
void message_box_set_visibility(int visible);

void message_box_draw();
void message_box_update();


#endif //___MESSAGE_BOX_H___