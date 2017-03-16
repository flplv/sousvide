#ifndef OWTS_H_
#define OWTS_H_

#include <stdint.h>

enum owts_action
{
    owts_wire_pull = 0,
    owts_wire_release = 1,
    owts_wire_acquire = 2,
};

enum conversion_result
{
    owts_conversion_success,
    owts_conversion_no_presence,
    owts_conversion_fifo_unexpectedly_empty
};

void owts_init ();
void owts_deinit ();
void owts_start_conversion ();

// Client defined functions, called from ISR
bool owts_drive_line (enum owts_action);
void owts_isr_register_call ( void (*cb)(uint16_t), uint16_t);
void owts_on_temperature (enum conversion_result, int32_t);

#endif // OWTS_H_
