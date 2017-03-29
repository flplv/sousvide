#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <ios.h>

enum pixel_color
{
    pixel_off = 0b00,
    pixel_green = 0b01,
    pixel_red = 0b10,
//    pixel_orange = pixel_green | pixel_red
};

static inline enum ios_pin ios_dpy_red (uint8_t line)
{
	extern enum ios_pin __ios_dpy_mapping [ios_dpy_groups_total][ios_dpy_line_total];
	return __ios_dpy_mapping [0][line];
}

static inline enum ios_pin ios_dpy_green (uint8_t line)
{
	extern enum ios_pin __ios_dpy_mapping [ios_dpy_groups_total][ios_dpy_line_total];
	return __ios_dpy_mapping [1][line];
}

static inline enum ios_pin ios_dpy_column (uint8_t column)
{
	extern enum ios_pin __ios_dpy_mapping [ios_dpy_groups_total][ios_dpy_line_total];
	return __ios_dpy_mapping [2][column];
}

static inline void ios_dpy_column_on (uint8_t col)
{
	ios_set (ios_dpy_column (col), 1);
}

static inline void ios_dpy_column_off (uint8_t col)
{
	ios_set (ios_dpy_column (col), 0);
}

static inline void ios_dpy_line_red_on (uint8_t line)
{
	ios_set (ios_dpy_red (line), 0);
}

static inline void ios_dpy_line_red_off (uint8_t line)
{
	ios_set (ios_dpy_red (line), 1);
}

static inline void ios_dpy_line_green_on (uint8_t line)
{
	ios_set (ios_dpy_green (line), 0);
}

static inline void ios_dpy_line_green_off (uint8_t line)
{
	ios_set (ios_dpy_green (line), 01);
}

static inline void ios_dpy_columns_mask (uint8_t data)
{
	ios_set (ios_dpy_column (0), (data >> 0) & 0x1);
	ios_set (ios_dpy_column (1), (data >> 1) & 0x1);
	ios_set (ios_dpy_column (2), (data >> 2) & 0x1);
	ios_set (ios_dpy_column (3), (data >> 3) & 0x1);
	ios_set (ios_dpy_column (4), (data >> 4) & 0x1);
	ios_set (ios_dpy_column (5), (data >> 5) & 0x1);
	ios_set (ios_dpy_column (6), (data >> 6) & 0x1);
	ios_set (ios_dpy_column (7), (data >> 7) & 0x1);
}

static inline void ios_dpy_line_red_mask (uint8_t data)
{
	ios_set (ios_dpy_red (0), !(data & (0x1 << 0)));
	ios_set (ios_dpy_red (1), !(data & (0x1 << 1)));
	ios_set (ios_dpy_red (2), !(data & (0x1 << 2)));
	ios_set (ios_dpy_red (3), !(data & (0x1 << 3)));
	ios_set (ios_dpy_red (4), !(data & (0x1 << 4)));
	ios_set (ios_dpy_red (5), !(data & (0x1 << 5)));
	ios_set (ios_dpy_red (6), !(data & (0x1 << 6)));
	ios_set (ios_dpy_red (7), !(data & (0x1 << 7)));
}

static inline void ios_dpy_line_green_mask (uint8_t data)
{
	ios_set (ios_dpy_green (0), !(data & (0x1 << 0)));
	ios_set (ios_dpy_green (1), !(data & (0x1 << 1)));
	ios_set (ios_dpy_green (2), !(data & (0x1 << 2)));
	ios_set (ios_dpy_green (3), !(data & (0x1 << 3)));
	ios_set (ios_dpy_green (4), !(data & (0x1 << 4)));
	ios_set (ios_dpy_green (5), !(data & (0x1 << 5)));
	ios_set (ios_dpy_green (6), !(data & (0x1 << 6)));
	ios_set (ios_dpy_green (7), !(data & (0x1 << 7)));
}

static inline void display_ios_periodic ()
{
	static uint8_t cc = 0;
	extern uint8_t __display_data_red [ios_dpy_col_total];
	extern uint8_t __display_data_green [ios_dpy_col_total];

	ios_dpy_column_off (cc);
	cc = (cc + 1) % ios_dpy_col_total;
	ios_dpy_line_red_mask (__display_data_red [cc]);
	ios_dpy_line_green_mask (__display_data_green [cc]);
	ios_dpy_column_on (cc);
}

void display_init ();
void display_draw_temperature (int16_t);
void display_draw_temperature_color (enum pixel_color);

void display_tick_clock (int16_t minutes);
void display_draw_clock (int16_t minutes);

void display_draw_underline (enum pixel_color);

/* User/platform provided functions */
void display_isr_start ();
void display_isr_stop ();

#endif // DISPLAY_H_
