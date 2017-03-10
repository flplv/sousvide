#include "fff.hpp"

extern "C"
{
    #include <owts.h>
    #include <error.h>
}

typedef void (*isr_callback_t)(uint16_t);
DECLARE_FAKE_VOID_FUNC2 (owts_isr_register_call, isr_callback_t, uint16_t);
DECLARE_FAKE_VALUE_FUNC (bool, owts_drive_line, enum owts_action);
DECLARE_FAKE_VOID_FUNC2 (owts_on_temperature, enum conversion_result, uint16_t);

DECLARE_FAKE_VOID_FUNC (error, enum errors);
