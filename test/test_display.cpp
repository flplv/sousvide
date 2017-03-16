#include "catch.hpp"
#include "fff.hpp"
#include "mocks.hpp"
#include <fl-lib.h>

extern "C"
{
    #include "display_glyphs.h"
}

namespace test_display
{
    /* White testing : ) */

    #include "display.c"

    void check_display_data (const uint8_t expected[FL_ARRAY_SIZE(display_data)])
    {
        for (int x = 0; x < 8; x ++)
            for (int y = 0; y < 8; y ++)
                // if (display_data[y * 8 + x] != expected[y * 8 + x])
                {
                    SCOPED_CAPTURE(x);
                    SCOPED_CAPTURE(y);
                    CHECK (expected[y * 8 + x] == display_data[y * 8 + x]);
                }
    }

    TEST_CASE ("display")
    {
        uint8_t expected_display_data [FL_ARRAY_SIZE(display_data)] = {0,};
        display_init ();
        check_display_data (expected_display_data);

        SECTION ("clock 0")
        {
            display_draw_clock (0);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("clock 1")
        {
            display_draw_clock (1);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("clock 10")
        {
            display_draw_clock (10);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("clock 59")
        {
            display_draw_clock (59);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            expected_display_data [0 + 5 * 8] = pixel_yellow;
            expected_display_data [0 + 4 * 8] = pixel_yellow;
            expected_display_data [0 + 3 * 8] = pixel_yellow;
            expected_display_data [0 + 2 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("clock 60")
        {
            display_draw_clock (60);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            expected_display_data [0 + 5 * 8] = pixel_yellow;
            expected_display_data [0 + 4 * 8] = pixel_yellow;
            expected_display_data [0 + 3 * 8] = pixel_yellow;
            expected_display_data [0 + 2 * 8] = pixel_yellow;
            expected_display_data [0 + 1 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("clock 100")
        {
            display_draw_clock (100);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            expected_display_data [0 + 5 * 8] = pixel_yellow;
            expected_display_data [0 + 4 * 8] = pixel_yellow;
            expected_display_data [0 + 3 * 8] = pixel_yellow;
            expected_display_data [0 + 2 * 8] = pixel_yellow;
            expected_display_data [0 + 1 * 8] = pixel_yellow;
            expected_display_data [0 + 0 * 8] = pixel_yellow;
            expected_display_data [1 + 0 * 8] = pixel_yellow;
            expected_display_data [2 + 0 * 8] = pixel_yellow;
            expected_display_data [3 + 0 * 8] = pixel_yellow;
            check_display_data (expected_display_data);
        }

        SECTION ("write horizontal full")
        {
            write_horizontal (3, 3, 4, pixel_red, 0xFF);
            expected_display_data [3 + 3 * 8] = pixel_red;
            expected_display_data [4 + 3 * 8] = pixel_red;
            expected_display_data [5 + 3 * 8] = pixel_red;
            expected_display_data [6 + 3 * 8] = pixel_red;
            check_display_data (expected_display_data);
        }

        SECTION ("write horizontal AA")
        {
            write_horizontal (3, 3, 4, pixel_red, 0xAA);
            expected_display_data [3 + 3 * 8] = pixel_red;
            expected_display_data [5 + 3 * 8] = pixel_red;
            check_display_data (expected_display_data);
        }

        SECTION ("write horizontal 81")
        {
            write_horizontal (0, 0, 8, pixel_red, 0x81);
            expected_display_data [0 + 0 * 8] = pixel_red;
            expected_display_data [7 + 0 * 8] = pixel_red;
            check_display_data (expected_display_data);
        }

        SECTION ("number one left")
        {
            display_draw_number (bitmap_number_one, glyph_left);
            expected_display_data [3 + 5 * 8] = pixel_green;
            expected_display_data [3 + 4 * 8] = pixel_green;
            expected_display_data [3 + 3 * 8] = pixel_green;
            expected_display_data [3 + 2 * 8] = pixel_green;
            expected_display_data [3 + 1 * 8] = pixel_green;
            check_display_data (expected_display_data);
        }

        SECTION ("number one right")
        {
            display_draw_number (bitmap_number_one, glyph_right);
            expected_display_data [6 + 5 * 8] = pixel_green;
            expected_display_data [6 + 4 * 8] = pixel_green;
            expected_display_data [6 + 3 * 8] = pixel_green;
            expected_display_data [6 + 2 * 8] = pixel_green;
            expected_display_data [6 + 1 * 8] = pixel_green;
            check_display_data (expected_display_data);
        }

        SECTION ("clock to pixel")
        {
            CHECK ( (0 + 7 * 8) == (int)clock_position_to_pixel(0));
            CHECK ( (0 + 6 * 8) == (int)clock_position_to_pixel(1));
            CHECK ( (0 + 0 * 8) == (int)clock_position_to_pixel(7));
            CHECK ( (1 + 0 * 8) == (int)clock_position_to_pixel(8));
            CHECK ( (6 + 0 * 8) == (int)clock_position_to_pixel(13));
            CHECK ( (7 + 0 * 8) == (int)clock_position_to_pixel(14));
            CHECK ( (7 + 1 * 8) == (int)clock_position_to_pixel(15));
            CHECK ( (7 + 6 * 8) == (int)clock_position_to_pixel(20));
            CHECK ( (7 + 7 * 8) == (int)clock_position_to_pixel(21));
            CHECK ( (6 + 7 * 8) == (int)clock_position_to_pixel(22));
            CHECK ( (1 + 7 * 8) == (int)clock_position_to_pixel(27));

            /* wrapping */
            CHECK ( (0 + 7 * 8) == (int)clock_position_to_pixel(28));
        }

        SECTION ("clock ticker")
        {
            uint8_t time = 0;

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_off;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            time = 10;
            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_off;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_yellow;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_off;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            time = 0;
            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_yellow;
            expected_display_data [0 + 6 * 8] = pixel_off;
            CAPTURE((int)time);
            check_display_data (expected_display_data);

            tick_clock (time);
            expected_display_data [0 + 7 * 8] = pixel_off;
            expected_display_data [0 + 6 * 8] = pixel_off;
            CAPTURE((int)time);
            check_display_data (expected_display_data);
        }
    }

}
