#ifndef SRC_UI_H_
#define SRC_UI_H_

#include <reacto/queue.h>

enum ui_event
{
    ui_event_left,
    ui_event_right,
    ui_event_click_up,
    ui_event_click_down,
};

void ui_init ();
void ui_temperature (int16_t temperature);

#define ui_buffer_size 16
int ui_handler (queue_t *);
void ui_push (enum ui_event);

#endif /* SRC_UI_H_ */
