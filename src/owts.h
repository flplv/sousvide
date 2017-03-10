#ifndef OWTS_H_
#define OWTS_H_

#include <stdint.h>

enum owts_action {
    owts_wire_pull = 0,
    owts_wire_release = 1,
    owts_wire_acquire = 2,
};

void owts_init ();
void owts_deinit ();

// Client defined functions, called from ISR
bool owts_drive_line (enum owts_action);
void owts_isr_register_call ( void (*cb)(uint16_t), uint16_t);

#endif // OWTS_H_
