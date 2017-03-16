#include "fff.hpp"
#include "mocks.hpp"

DEFINE_FAKE_VOID_FUNC2 (owts_isr_register_call, isr_callback_t, uint16_t);
DEFINE_FAKE_VALUE_FUNC (bool, owts_drive_line, enum owts_action);
DEFINE_FAKE_VOID_FUNC2 (owts_on_temperature, enum conversion_result, int32_t);

DEFINE_FAKE_VOID_FUNC (error, enum errors);
