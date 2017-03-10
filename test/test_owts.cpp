#include "catch.hpp"
#include "fff.hpp"
#include "mocks.hpp"

/* One Wire Temperature Sensor test */

namespace test_owts
{
    extern "C"
    {
        /* White testing : ) */
        #include "owts.c"
    }

    TEST_CASE ("owts")
    {
        RESET_FAKE (owts_isr_register_call);
        RESET_FAKE (owts_drive_line);
        RESET_FAKE (error);
        owts_init ();

        SECTION ("reset")
        {
            pulse_initialize ();
            owts_on_timeout (0);
            owts_on_timeout (0);

            SECTION ("success presence ")
            {
                owts_drive_line_fake.return_val = 0;
                owts_on_timeout (0);

                CHECK (owts_drive_line_fake.call_count == 3);
                CHECK (owts_drive_line_fake.arg0_history[0] == owts_wire_pull);
                CHECK (owts_drive_line_fake.arg0_history[1] == owts_wire_release);
                CHECK (owts_drive_line_fake.arg0_history[2] == owts_wire_acquire);

                CHECK (owts_isr_register_call_fake.call_count == 3);
                CHECK (owts_isr_register_call_fake.arg0_history[0] == &owts_on_timeout);
                CHECK (owts_isr_register_call_fake.arg0_history[1] == &owts_on_timeout);
                CHECK (owts_isr_register_call_fake.arg0_history[2] == &owts_on_timeout);
                CHECK (owts_isr_register_call_fake.arg1_history[0] == period_reset);
                CHECK (owts_isr_register_call_fake.arg1_history[1] == period_tx_presence);
                CHECK (owts_isr_register_call_fake.arg1_history[2] == period_reset);

                CHECK (error_fake.call_count == 0);
            }
            SECTION ("fail presence")
            {
                /* No presence reply */
                owts_drive_line_fake.return_val = 1;

                owts_on_timeout (0);
                CHECK (error_fake.call_count == 1);
                CHECK (error_fake.arg0_val == error_owts_no_presence);
            }
        }

        owts_deinit ();
    }

}
