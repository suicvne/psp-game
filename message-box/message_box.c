#include "message_box.h"

const int MESSAGE_BOX_MAX_CHAR_LENGTH = 255; //maximum characters per line for a message
const int MESSAGE_BOX_MAX_LINES = 3; //maximum lines per single message box
const int MESSAGE_BOX_MAX_WIDTH = 300; //maximum width in pixels that the message box should be.
const int MESSAGE_BOX_MAX_HEIGHT = 50; //maximum height in pixels that the message box should be.

const int MESSAGE_BOX_ORIGIN_X = 240 - 150; //where the message box is to start drawing
const int MESSAGE_BOX_ORIGIN_Y = 220; //where the message box is to start drawing

int message_box_framecount = 0;
int message_box_is_visible = 0;

rectangle_t message_box_rectangle;

int message_box_text_index = 0;

char* message_box_message = "no.";

void message_box_set_message(const char* message)
{
    if(message_box_message != NULL)
    {/* TODO? probably not */}

    int size = sizeof(char) * strlen(message) + 1;
    message_box_message = malloc(size);
    memset(message_box_message, 0, size); //zero off
    memcpy(message_box_message, message, strlen(message));
    printf("message box message set to: '%s'\n", message_box_message);
} 

void message_box_set_visibility(int visible)
{
    message_box_is_visible = (visible == 1) ? 1 : 0;
    if(visible) //setting to visible
    {
        message_box_rectangle.x = MESSAGE_BOX_ORIGIN_X;
        message_box_rectangle.y = MESSAGE_BOX_ORIGIN_Y;
        message_box_rectangle.w = MESSAGE_BOX_MAX_WIDTH;
        message_box_rectangle.h = MESSAGE_BOX_MAX_HEIGHT;
    }
}

void message_box_draw()
{
    if(message_box_is_visible)
    {
        //printf("message box draw: %d, %d (%d x %d)\n", message_box_rectangle.x, message_box_rectangle.y, message_box_rectangle.w, message_box_rectangle.h);
        rectangle_draw_filled(&message_box_rectangle, RGBA(0, 0, 0, 255));
        int i, message_box_y_offset = 0;
        int id = 0;
        for(i = 0; i < message_box_text_index; i++)
        {
            char c = message_box_message[i];
            int draw_x = MESSAGE_BOX_ORIGIN_X + 3 + (id * 6);
            
            if(draw_x > (MESSAGE_BOX_MAX_WIDTH + MESSAGE_BOX_ORIGIN_X) - 8 || c == '\n')
            {
                message_box_y_offset += 12;
                id = 0;
                draw_x = MESSAGE_BOX_ORIGIN_X + 3 + (id * 6);
            }
            id++;
            text_render_text(&c, draw_x, MESSAGE_BOX_ORIGIN_Y + message_box_y_offset);
        }
    }
}

void message_box_update()
{
    if(message_box_is_visible) //why use extra cycles if it's not visible?'
    {
        message_box_framecount++;
        if(message_box_framecount > 5)
        {
            message_box_framecount = 0;
            message_box_text_index++;
            if(message_box_text_index > strlen(message_box_message))
                message_box_text_index--;
        }
    }
}