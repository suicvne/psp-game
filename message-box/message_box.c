#include "message_box.h"

const int MESSAGE_BOX_MAX_CHAR_LENGTH = 255; //maximum characters per line for a message
const int MESSAGE_BOX_MAX_LINES = 3; //maximum lines per single message box
const int MESSAGE_BOX_MAX_WIDTH = 300; //maximum width in pixels that the message box should be.
const int MESSAGE_BOX_MAX_HEIGHT = 150; //maximum height in pixels that the message box should be.

const int MESSAGE_BOX_ORIGIN_X = 0; //where the message box is to start drawing
const int MESSAGE_BOX_ORIGIN_Y = 0; //where the message box is to start drawing


int message_box_framecount = 0;
int message_box_is_visible = 0;

int message_box_current_width = 0;
int message_box_current_height = 0;

char* message_box_message = "no.";

void message_box_set_message(const char* message)
{
    if(message_box_message != NULL)
    {
        //printf("msg box free\n");
        //free(message_box_message);
    }

    int size = sizeof(char) * strlen(message) + 1;
    message_box_message = malloc(size);
    memset(message_box_message, 0, size); //zero off
    memcpy(message_box_message, message, strlen(message));
    printf("message box message set to: '%s'\n", message_box_message);
} 

void message_box_set_visibility(int visible)
{
    message_box_is_visible = (visible == 1) ? 1 : 0;
}

void message_box_draw()
{
    if(message_box_is_visible)
    {
        printf("message box draw\n");
    }
}

void message_box_update()
{
    if(message_box_is_visible) //why use extra cycles if it's not visible?'
    {
        printf("message box update\n");
    }
}