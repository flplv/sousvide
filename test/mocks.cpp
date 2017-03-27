#include "fff.hpp"
#include "mocks.hpp"

/* ios module */
DEFINE_FAKE_VOID_FUNC (ios_set, enum ios_pin, bool);
DEFINE_FAKE_VALUE_FUNC (bool, ios_get, enum ios_pin);
DEFINE_FAKE_VOID_FUNC (ios_set_output, enum ios_pin);
DEFINE_FAKE_VOID_FUNC (ios_set_input, enum ios_pin);

/* display module */
DEFINE_FAKE_VOID_FUNC (display_isr_start);
DEFINE_FAKE_VOID_FUNC (display_isr_stop);

/* owts module */
DEFINE_FAKE_VOID_FUNC (owts_isr_stop);
DEFINE_FAKE_VOID_FUNC (owts_isr_start, isr_callback_t);
DEFINE_FAKE_VOID_FUNC (owts_isr_next_call, uint16_t);
DEFINE_FAKE_VOID_FUNC (owts_on_temperature, enum conversion_result, int32_t);

///* reacto framework */
//DEFINE_FAKE_VALUE_FUNC (reacto_time_t, time_now);
//DEFINE_FAKE_VOID_FUNC (hardware_watchdog_init);
//DEFINE_FAKE_VOID_FUNC (hardware_watchdog_kick);
//DEFINE_FAKE_VOID_FUNC (hardware_watchdog_deinit);
