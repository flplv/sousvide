#include <string.h>
#include <stdint.h>
#include "fl-lib.h"
#include "display_glyphs.h"

/* 8 per 8 dot matrix display, two color */
enum pixel_color
{
    pixel_off = 0b00,
    pixel_green = 0b01,
    pixel_red = 0b10,
    pixel_yellow = pixel_green | pixel_red
};

enum glyph_position
{
    glyph_left,
    glyph_right,
};

static uint8_t display_data[8 * 8];

static void write_horizontal (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color, uint8_t data)
{
    for (int bit = 7; bit >= (8-len); -- bit)
    {
        if (data & (0x1 << bit))
            display_data [y * 8 + x] = color;
        else
            display_data [y * 8 + x] = pixel_off;

        x ++;
    }
}

static void write_dots_upward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
        display_data [y * 8 + x] = color;
        y -= 1;
    }
}

static void write_dots_downward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
        display_data [y * 8 + x] = color;
        y += 1;
    }
}

static void write_dots_rightward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
        display_data [y * 8 + x] = color;
        x += 1;
    }
}

static void write_dots_leftward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
        display_data [y * 8 + x] = color;
        x -= 1;
    }
}

static void display_draw_number (const uint8_t * number, enum glyph_position pos)
{
    const uint8_t x = pos == glyph_left ? 1 : 4;
    const uint8_t y = 1;

    for (int i = 0; i < GLYPH_DATA_LEN; i ++)
        write_horizontal (x, y + i, 3, pixel_green, number[i]);
}

static void display_draw_symbol (const uint8_t * symbol)
{
    const uint8_t x = 1;
    const uint8_t y = 1;

    for (int i = 0; i < GLYPH_DATA_LEN; i ++)
        write_horizontal (x, y + i, 6, pixel_green, symbol[i]);
}

static void display_draw_clock (uint8_t minutes)
{
    int8_t dots = (minutes / 10) + 1;

    if (dots > 0)
        write_dots_upward (0, 7, dots > 8 ? 8 : dots, pixel_yellow);
    dots -= 8;

    if (dots > 0)
        write_dots_rightward (1, 0, dots > 6 ? 6 : dots, pixel_yellow);
    dots -= 6;

    if (dots > 0)
        write_dots_downward (7, 0, dots > 8 ? 8 : dots, pixel_yellow);
    dots -= 8;

    if (dots > 0)
        write_dots_leftward (7, 7, dots > 6 ? 6 : dots, pixel_yellow);
}

static uint8_t clock_position_to_pixel (uint8_t dots)
{
    uint8_t x;
    uint8_t y;

    if (dots >= 21)
    {
        x = 7 - (dots - 21);
        y = 7;
    }
    else if (dots >= 14)
    {
        x = 7;
        y = (dots - 14);
    }
    else if (dots >= 7)
    {
        x = (dots - 7);
        y = 0;
    }
    else
    {
        x = 0;
        y = 7 - dots;
    }

    return x + 8 * y;
}

static void tick_clock (uint8_t minutes)
{
    static uint8_t last = 1;
    uint8_t curr = (minutes / 10) + 1;

    if (last < curr)
        display_draw_clock (minutes);
    else if (last > curr)
        display_data [clock_position_to_pixel (last - 1)] = pixel_off;
    else
        display_data [clock_position_to_pixel (curr - 1)] ^= pixel_yellow;

    last = curr;
}

void display_init ()
{
    memset (display_data, 0x00, FL_ARRAY_SIZE (display_data));
}
