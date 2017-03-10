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
        RESET_FAKE (owts_on_temperature);
        RESET_FAKE (error);
        owts_init ();

        SECTION ("conversion")
        {
            owts_start_conversion ();
            owts_on_timeout (0);
            owts_on_timeout (0);

            SECTION ("fail presence")
            {
                /* No presence reply */
                owts_drive_line_fake.return_val = 1;
                owts_on_timeout (0);

                CHECK (owts_on_temperature_fake.call_count == 1);
                CHECK (owts_on_temperature_fake.arg0_val == owts_conversion_no_presence);
                CHECK (owts_on_temperature_fake.arg1_val == 0);
            }

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

                CHECK (owts_on_temperature_fake.call_count == 0);

                int n = 8 * 2;
                while (n--)
                {
                    CHECK (event_buffer_peek ()->action == owts_wire_pull);
                    owts_on_timeout (0);
                    CHECK (event_buffer_peek ()->action == owts_wire_release);
                    owts_on_timeout (0);
                }

                SECTION ("wait conversion")
                {
                    int read_pos = d.event_buffer_read_pos;

                    owts_drive_line_fake.return_val = 0;
                    n = 100;
                    while (n--)
                    {
                        CHECK (event_buffer_peek ()->action == owts_wire_pull);
                        owts_on_timeout (0);
                        CHECK (event_buffer_peek ()->action == owts_wire_release);
                        owts_on_timeout (0);
                        CHECK (event_buffer_peek ()->action == owts_wire_acquire);
                        owts_on_timeout (0);

                        CHECK ((int)d.event_buffer_read_pos == read_pos);
                    }

                    owts_drive_line_fake.return_val = 1;
                    CHECK (event_buffer_peek ()->action == owts_wire_pull);
                    owts_on_timeout (0);
                    CHECK (event_buffer_peek ()->action == owts_wire_release);
                    owts_on_timeout (0);
                    CHECK (event_buffer_peek ()->action == owts_wire_acquire);
                    owts_on_timeout (0);
                    CHECK (d.event_buffer_read_pos != read_pos);

                    /* Skip second initialization */
                    owts_drive_line_fake.return_val = 0;
                    owts_on_timeout (0);
                    owts_on_timeout (0);
                    owts_on_timeout (0);

                    int n = 8 * 2;
                    while (n--)
                    {
                        CHECK (event_buffer_peek ()->action == owts_wire_pull);
                        owts_on_timeout (0);
                        CHECK (event_buffer_peek ()->action == owts_wire_release);
                        owts_on_timeout (0);
                    }

                    SECTION ("read temperature")
                    {
                        uint16_t expected_raw_temperature = 0xBEEF;

                        n = 8 * 2;
                        for (int i = 0; i < n; i++)
                        {
                            owts_drive_line_fake.return_val = (expected_raw_temperature >> i) & 0x1;

                            CHECK (event_buffer_peek ()->action == owts_wire_pull);
                            owts_on_timeout (0);
                            CHECK (event_buffer_peek ()->action == owts_wire_release);
                            owts_on_timeout (0);
                            CHECK (event_buffer_peek ()->action == owts_wire_acquire);
                            owts_on_timeout (0);
                        }

                        /* Skip last initialization, and the fifo must be empty */
                        owts_drive_line_fake.return_val = 0;
                        owts_on_timeout (0);
                        owts_on_timeout (0);
                        owts_on_timeout (0);

                        CHECK (event_buffer_empty () == true);
                        CHECK (owts_on_temperature_fake.call_count == 1);
                        CHECK (owts_on_temperature_fake.arg0_val == owts_conversion_success);
                        CHECK (owts_on_temperature_fake.arg1_val == expected_raw_temperature);
                    }
                }
            }
        }

        owts_deinit ();
    }

}
