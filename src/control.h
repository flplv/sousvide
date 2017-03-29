#ifndef SRC_CONTROL_H_
#define SRC_CONTROL_H_

#include <stdint.h>

int16_t control_get_setpoint ();
void control_set_setpoint (int16_t);

int16_t control_get_timer ();
void control_set_timer (int16_t);

void control_process (int16_t current_temperature);

#endif /* SRC_CONTROL_H_ */
