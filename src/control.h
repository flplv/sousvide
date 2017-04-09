#ifndef SRC_CONTROL_H_
#define SRC_CONTROL_H_

#include <stdint.h>
#include <reacto/queue.h>

int16_t control_get_setpoint ();
void control_set_setpoint (int16_t);

int16_t control_get_timer ();
void control_set_timer (int16_t);

bool control_is_on ();

#define control_buffer_size 8
int control_handler (queue_t * queue);

void control_init ();

#endif /* SRC_CONTROL_H_ */
