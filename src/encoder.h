#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <ios.h>

void encoder_init ();
void encoder_input_event (enum ios_input_event event, enum ios_pin pin);

#endif /* SRC_ENCODER_H_ */
