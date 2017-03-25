#include <string.h>
#include <stdint.h>
#include "fl-lib.h"
#include "display_glyphs.h"
#include "display.h"
#include "ios.h"

/* 8 per 8 dot matrix display, two color */
enum pixel_color
{
    pixel_off = 0b00,
    pixel_green = 0b01,
    pixel_red = 0b10,
//    pixel_orange = pixel_green | pixel_red
};

enum glyph_position
{
    glyph_left,
    glyph_right,
};

uint8_t __display_data_red [ios_dpy_col_total];
uint8_t __display_data_green [ios_dpy_col_total];

static void xor_pixel (uint8_t x, uint8_t y, enum pixel_color color)
{
	uint8_t bit_buffer_data = 0x1 << y;

	if (color & pixel_red)
		__display_data_red [x] ^=  bit_buffer_data;

	if (color & pixel_green)
		__display_data_green [x] ^=  bit_buffer_data;
}

static void set_pixel (uint8_t x, uint8_t y, enum pixel_color color)
{
	uint8_t bit_buffer_data = 0x1 << y;

	__display_data_red [x] &= ~bit_buffer_data;
	__display_data_green [x] &= ~bit_buffer_data;

	if (color & pixel_red)
		__display_data_red [x] |=  bit_buffer_data;

	if (color & pixel_green)
		__display_data_green [x] |=  bit_buffer_data;
}

static void write_horizontal (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color, uint8_t data)
{
    for (int bit = 7; bit >= (8-len); -- bit)
    {
    	set_pixel (x, y, data & (0x1 << bit) ? color : pixel_off);
		x ++;
    }
}

static void write_dots_upward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
    	set_pixel (x, y, color);
        y -= 1;
    }
}

static void write_dots_downward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
    	set_pixel (x, y, color);
        y += 1;
    }
}

static void write_dots_rightward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
    	set_pixel (x, y, color);
        x += 1;
    }
}

static void write_dots_leftward (uint8_t x, uint8_t y, uint8_t len, enum pixel_color color)
{
    for (int i = 0; i < len; i ++)
    {
    	set_pixel (x, y, color);
        x -= 1;
    }
}

static void display_draw_number (const uint8_t * number, enum glyph_position pos)
{
    const uint8_t x = pos == glyph_left ? 1 : 4;
    const uint8_t y = 1;

    for (int i = 0; i < GLYPH_DATA_LEN; i ++)
        write_horizontal (x, y + i, 3, pixel_red, number[i]);
}

static void display_draw_symbol (const uint8_t * symbol)
{
    const uint8_t x = 1;
    const uint8_t y = 1;

    for (int i = 0; i < GLYPH_DATA_LEN; i ++)
        write_horizontal (x, y + i, 6, pixel_red, symbol[i]);
}

static void display_draw_clock (uint8_t minutes)
{
    int8_t dots = (minutes / 10) + 1;

    if (dots > 0)
        write_dots_upward (0, 7, dots > 8 ? 8 : dots, pixel_green);
    dots -= 8;

    if (dots > 0)
        write_dots_rightward (1, 0, dots > 6 ? 6 : dots, pixel_green);
    dots -= 6;

    if (dots > 0)
        write_dots_downward (7, 0, dots > 8 ? 8 : dots, pixel_green);
    dots -= 8;

    if (dots > 0)
        write_dots_leftward (7, 7, dots > 6 ? 6 : dots, pixel_green);
}

static void clock_position_to_pixel (uint8_t dots, uint8_t * xout, uint8_t * yout)
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

    *xout = x;
    *yout = y;
}

void display_tick_clock (uint8_t minutes)
{
    static uint8_t last = 1;
    uint8_t curr = (minutes / 10) + 1;

    if (last < curr)
    {
        display_draw_clock (minutes);
    }
    else if (last > curr)
    {
    	uint8_t x,y;
    	clock_position_to_pixel (last - 1, &x, &y);
    	set_pixel (x, y, pixel_off);
    }
	else
	{
    	uint8_t x,y;
    	clock_position_to_pixel (last - 1, &x, &y);
    	xor_pixel (x, y, pixel_green);
	}
    last = curr;
}

void display_set_temperature (uint8_t temp)
{
	if (temp > 99)
	{
	    display_draw_number (bitmap_number_one, glyph_left);
	    display_draw_number (bitmap_number_dash, glyph_right);
	}
	else
	{
		uint8_t l, r;
		l = temp / 10;
		r = temp % 10;

		display_draw_number (bitmap_digits[l], glyph_left);
	    display_draw_number (bitmap_digits[r], glyph_right);
	}
}

void display_init ()
{
    memset (__display_data_red, 0x00, FL_ARRAY_SIZE (__display_data_red));
    memset (__display_data_green, 0x00, FL_ARRAY_SIZE (__display_data_green));

	for (int cc = 0; cc < ios_dpy_col_total; cc ++)
	{
		ios_set (ios_dpy_column (cc), 0);
		ios_set_output (ios_dpy_column (cc));
	}

	for (int ll = 0; ll < ios_dpy_line_total; ll ++)
	{
		ios_set (ios_dpy_red (ll), 1);
		ios_set_output (ios_dpy_red (ll));
	}

	for (int ll = 0; ll < ios_dpy_line_total; ll ++)
	{
		ios_set (ios_dpy_green (ll), 1);
		ios_set_output (ios_dpy_green (ll));
	}

	display_draw_number (bitmap_number_dash, glyph_left);
	display_draw_number (bitmap_number_dash, glyph_right);
    display_isr_start ();
}
