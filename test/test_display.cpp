#include "catch.hpp"
#include "fff.hpp"
#include "mocks.hpp"
#include <fl-lib.h>

extern "C"
{
    #include <display.c>
}

void check_display_data (const uint8_t expected[ios_dpy_col_total],
        const uint8_t real[ios_dpy_col_total])
{
    for (int x = 0; x < 8; x ++)
        for (int y = 0; y < 8; y ++)
            // if (display_data[y * 8 + x] != expected[y * 8 + x])
            {
                SCOPED_CAPTURE(x);
                SCOPED_CAPTURE(y);
                CHECK ((expected[x] & (0x1 << y)) == (real[x] & (0x1 << y)));
            }
}

TEST_CASE ("display")
{
    uint8_t expected_display_data_red [ios_dpy_col_total] = {
            0x0, 0x8, 0x8, 0x8,
            0x8, 0x8, 0x8, 0x0
    };
    uint8_t expected_display_data_green [ios_dpy_col_total] = {0,};

    RESET_FAKE (display_isr_start);
    RESET_FAKE (display_isr_stop);


    display_init ();

    CHECK (display_isr_start_fake.call_count == 1);
    CHECK (display_isr_stop_fake.call_count == 0);

    check_display_data (expected_display_data_green, __display_data_green);
    check_display_data (expected_display_data_red, __display_data_red);

    SECTION ("clock 0")
    {
        display_draw_clock (0);
        expected_display_data_green [0] = 0x80;
        check_display_data (expected_display_data_green, __display_data_green);
    }

    SECTION ("clock 1")
    {
        display_draw_clock (1);
        expected_display_data_green [0] = 0x80;
        check_display_data (expected_display_data_green, __display_data_green);
    }

    SECTION ("clock 10")
    {
        display_draw_clock (10);
        expected_display_data_green [0] = 0xC0;
        check_display_data (expected_display_data_green, __display_data_green);
    }

    SECTION ("clock 59")
    {
        display_draw_clock (59);
        expected_display_data_green [0] = 0xFC;
        check_display_data (expected_display_data_green, __display_data_green);
    }

    SECTION ("clock 60")
    {
        display_draw_clock (60);
        expected_display_data_green [0] = 0xFE;
        check_display_data (expected_display_data_green, __display_data_green);
    }
}
