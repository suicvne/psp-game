#include "message_box.h"

// Queue Related
int message_box_queue_index = 0;
message_box_t* message_box_queue;
const int MESSAGE_BOX_MAX_QUEUE_SIZE = 10;
// Queue Related

/**
Misc Constants / older variables
*/
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
/**
Misc Constants / older variables
*/

int message_box_has_messages()
{
    int i;
    for(i = 0; i < MESSAGE_BOX_MAX_QUEUE_SIZE; i++)
    {
        message_box_t box = message_box_queue[i];
        if(box.shown)
            return 1;
    }

    return 0;
}

void message_box_init_queue()
{
    int size = sizeof(message_box_t) * MESSAGE_BOX_MAX_QUEUE_SIZE;
    message_box_queue = malloc(size);
    memset(message_box_queue, 0, MESSAGE_BOX_MAX_QUEUE_SIZE); //zero off the array
    printf("msg box queue init\n");
}

void message_box_queue_push(message_box_t message)
{
    if((message_box_queue_index + 1) > MESSAGE_BOX_MAX_QUEUE_SIZE)
        return; //no overflows pls
    printf("push\n");
    printf("given: %s\n", message.message);
    message_box_queue[message_box_queue_index] = message;
    //memcpy(message_box_queue[message_box_queue_index], message, message_box_queue_index);
    printf("good: %s\n", message_box_queue[message_box_queue_index].message);
}

void message_box_queue_pop(int index)
{
    if(index > MESSAGE_BOX_MAX_QUEUE_SIZE)
        return;
    //reorder the array
    int i;
    for(i = index; i < MESSAGE_BOX_MAX_QUEUE_SIZE - 1; i++)
    {
        message_box_queue[i] = message_box_queue[i + 1];
    }
}

void message_box_create(const char* message)
{
    if(message_box_queue == NULL)
        message_box_init_queue();


    message_box_t message_box;

    int size = sizeof(char) * strlen(message) + 1; //+ 1 for null
    message_box.message = malloc(size);
    memset(message_box.message, 0, size);
    memcpy(message_box.message, message, strlen(message));

    message_box.framecount = 0;
    message_box.current_index = 0;
    message_box.shown = 0;

    printf("created message box with message '%s'\n", message_box.message);

    message_box_queue_push(message_box);
}

void message_box_set_message(const char* message)
{
    if(message_box_message != NULL)
    {/* TODO? probably not */}
    if(message_box_is_visible)
        return;

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
        kUpdate = 0; //no update
    }
    else
    {
        message_box_text_index = 0;
        kUpdate = 1;
    }
}

void message_box_draw()
{
    if(message_box_is_visible) //should trigger the queue and begin to take from it
    {
        if(message_box_queue_index >= 0 && message_box_queue_index < MESSAGE_BOX_MAX_QUEUE_SIZE)
        {
            message_box_t box = message_box_queue[message_box_queue_index];

            rectangle_draw_filled(&message_box_rectangle, RGBA(0, 0, 0, 255));
            int i, message_box_y_offset = 0;
            int id = 0;
            for(i = 0; i < box.current_index; i++)
            {
                //char c = message_box_message[i];
                char c = box.message[i]; //wew

                int draw_x = MESSAGE_BOX_ORIGIN_X + 3 + (id * 6);

                if(c == '\n')
                {
                    message_box_y_offset += 12;
                    id = 0;
                    continue;
                }
                if(draw_x > (MESSAGE_BOX_MAX_WIDTH + MESSAGE_BOX_ORIGIN_X) - 8)
                {
                    message_box_y_offset += 12;
                    id = 0;
                    draw_x = MESSAGE_BOX_ORIGIN_X + 3 + (id * 6);
                }
                id++;
                text_render_char(c, draw_x, MESSAGE_BOX_ORIGIN_Y + message_box_y_offset);
            }
        }
        else
            message_box_set_visibility(0);
    }
}

void message_box_update()
{
    if(message_box_is_visible) //why use extra cycles if it's not visible?'
    {
        if(message_box_queue_index >= 0 && message_box_queue_index < MESSAGE_BOX_MAX_QUEUE_SIZE)
        {
            message_box_t box = message_box_queue[message_box_queue_index];
            box.framecount++;
            int frame_mod = 5;
            if(input_current_frame.button_interact)
                frame_mod = 1;

            if(box.framecount > frame_mod)
            {
                box.framecount = 0;
                box.current_index++;
                if(box.current_index > strlen(box.message))
                    box.current_index--;
                else
                {
                    if(box.message[message_box_text_index] != ' ')
                        sound_play(SOUND_EFFECT_TEXT);
                }
                if(box.current_index >= (strlen(message_box_message) - 1))
                {
                    if(input_is_button_just_pressed(INPUT_BUTTON_INTERACT))
                    {
                        message_box_queue_pop(message_box_queue_index);
                        message_box_queue_index++;
                        if(!message_box_has_messages())
                            message_box_is_visible = 0;
                    }
                }
            }
            message_box_queue[message_box_queue_index] = box;
        }
    }
}
