#include "catch.hpp"
#include "fff.hpp"
#include "mocks.hpp"

/* One Wire Temperature Sensor test */
extern "C"
{
    #include <owts.c>
}

TEST_CASE ("owts")
{
    RESET_FAKE (owts_isr_next_call);
    RESET_FAKE (owts_isr_start);
    RESET_FAKE (owts_isr_stop);
    RESET_FAKE (owts_on_temperature);
    RESET_FAKE (ios_get);
    RESET_FAKE (ios_set);
    RESET_FAKE (ios_set_input);
    RESET_FAKE (ios_set_output);

    owts_init ();
    CHECK (owts_isr_stop_fake.call_count == 1);
    CHECK (owts_isr_start_fake.call_count == 0);
    CHECK (ios_set_input_fake.arg0_history[0] == ios_owts_data);
    CHECK (ios_set_fake.arg0_history[0] == ios_owts_data);
    CHECK (ios_set_fake.arg1_history[0] == false);
    CHECK (ios_get_fake.call_count == 0);

    owts_start_conversion ();
    CHECK (owts_isr_start_fake.arg0_val == &owts_on_timeout);
    CHECK (owts_isr_start_fake.call_count == 1);
    CHECK (owts_isr_next_call_fake.call_count == 1);

    RESET_FAKE (ios_get);
    RESET_FAKE (ios_set);
    RESET_FAKE (ios_set_input);
    RESET_FAKE (ios_set_output);
    RESET_FAKE (owts_isr_next_call);

    SECTION ("conversion")
    {
        owts_on_timeout (0);
        owts_on_timeout (0);

        SECTION ("fail presence")
        {
            /* No presence reply */
            ios_get_fake.return_val = 1;
            owts_on_timeout (0);

            CHECK (owts_on_temperature_fake.call_count == 1);
            CHECK (owts_on_temperature_fake.arg0_val == owts_conversion_no_presence);
            CHECK (owts_on_temperature_fake.arg1_val == 0);
        }

        SECTION ("success presence ")
        {
            ios_get_fake.return_val = 0;
            owts_on_timeout (0);

            CHECK (ios_get_fake.call_count == 3);
            CHECK (owts_isr_next_call_fake.call_count == 3);
            CHECK (owts_isr_next_call_fake.arg0_history[0] == period_reset);
            CHECK (owts_isr_next_call_fake.arg0_history[1] == period_tx_presence);
            CHECK (owts_isr_next_call_fake.arg0_history[2] == period_reset);

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

                ios_get_fake.return_val = 0;
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

                ios_get_fake.return_val = 1;
                CHECK (event_buffer_peek ()->action == owts_wire_pull);
                owts_on_timeout (0);
                CHECK (event_buffer_peek ()->action == owts_wire_release);
                owts_on_timeout (0);
                CHECK (event_buffer_peek ()->action == owts_wire_acquire);
                owts_on_timeout (0);
                CHECK (d.event_buffer_read_pos != read_pos);

                /* Skip second initialization */
                ios_get_fake.return_val = 0;
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
                    int32_t expected_temperature = ((int16_t)0xBEEF >> 1) * 125;

                    n = 8 * 2;
                    for (int i = 0; i < n; i++)
                    {
                        ios_get_fake.return_val = (expected_raw_temperature >> i) & 0x1;

                        CHECK (event_buffer_peek ()->action == owts_wire_pull);
                        owts_on_timeout (0);
                        CHECK (event_buffer_peek ()->action == owts_wire_release);
                        owts_on_timeout (0);
                        CHECK (event_buffer_peek ()->action == owts_wire_acquire);
                        owts_on_timeout (0);
                    }

                    /* Skip last initialization, and the fifo must be empty */
                    ios_get_fake.return_val = 0;
                    owts_on_timeout (0);
                    owts_on_timeout (0);
                    owts_on_timeout (0);

                    CHECK (event_buffer_empty () == true);
                    CHECK (owts_on_temperature_fake.call_count == 1);
                    CHECK (owts_on_temperature_fake.arg0_val == owts_conversion_success);
                    CHECK (owts_on_temperature_fake.arg1_val == expected_temperature);
                }
            }
        }
    }

    owts_deinit ();
}
