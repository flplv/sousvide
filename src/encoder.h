#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <ios.h>

enum encoder_event
{
    encoder_event_left,
    encoder_event_right,
    encoder_event_push
};

void encoder_init ();
void encoder_input_scroll (bool dt);
void encoder_input_push ();

#endif /* SRC_ENCODER_H_ */
