#include "fff.hpp"

extern "C"
{
    #include <owts.h>
    #include <error.h>
    #include <ios.h>
    #include <reacto/reusables/time.h>
}

/* ios module */
DECLARE_FAKE_VOID_FUNC (ios_set, enum ios_pin, bool);
DECLARE_FAKE_VALUE_FUNC (bool, ios_get, enum ios_pin);
DECLARE_FAKE_VOID_FUNC (ios_set_output, enum ios_pin);
DECLARE_FAKE_VOID_FUNC (ios_set_input, enum ios_pin);

/* display module */
DECLARE_FAKE_VOID_FUNC (display_isr_start);
DECLARE_FAKE_VOID_FUNC (display_isr_stop);

/* owts module */
typedef void (*isr_callback_t)(uint16_t);
DECLARE_FAKE_VOID_FUNC (owts_isr_stop);
DECLARE_FAKE_VOID_FUNC (owts_isr_start, isr_callback_t);
DECLARE_FAKE_VOID_FUNC (owts_isr_next_call, uint16_t);
DECLARE_FAKE_VOID_FUNC (owts_on_temperature, enum conversion_result, int32_t);

///* reacto framework */
//DECLARE_FAKE_VALUE_FUNC (reacto_time_t, time_now);
//DECLARE_FAKE_VOID_FUNC (hardware_watchdog_init);
//DECLARE_FAKE_VOID_FUNC (hardware_watchdog_kick);
//DECLARE_FAKE_VOID_FUNC (hardware_watchdog_deinit);
