/**

Contains various functions for in-game message boxes.

*/

#ifndef ___MESSAGE_BOX_H___
#define ___MESSAGE_BOX_H___

#include <stdio.h>
#include <string.h>

#include "../sound/sound.h"
#include "../globals.h"
#include "../input/input.h"
#include "../graphics/rectangle.h"
#include "../graphics/text.h"

typedef struct message_box_t {

    char* message; //the actual message to print to the screen
    int framecount; //the amount of frames the message box has been on screen
    int current_index; //the current message index the message box is at
    int shown; //if 1, the message was shown already.

} message_box_t;

extern message_box_t* message_box_queue;
extern const int MESSAGE_BOX_MAX_QUEUE_SIZE;

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

/**
creates a new message_box_t* and adds it to the queue
*/
void message_box_create(const char* message);

void message_box_set_message(const char* message); //copies the specified message into the current message variable 
void message_box_set_visibility(int visible);

void message_box_draw();
void message_box_update();


#endif //___MESSAGE_BOX_H___