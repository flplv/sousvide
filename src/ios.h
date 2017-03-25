#ifndef SRC_IOS_H_
#define SRC_IOS_H_

#include <stdint.h>
#include <stdbool.h>

enum __attribute__ ((__packed__)) ios_pin
{
	ios_port_0_pin_0  = 0x00 | 0,
	ios_port_0_pin_1  = 0x00 | 1,
	ios_port_0_pin_2  = 0x00 | 2,
	ios_port_0_pin_3  = 0x00 | 3,
	ios_port_0_pin_4  = 0x00 | 4,
	ios_port_0_pin_5  = 0x00 | 5,
	ios_port_0_pin_6  = 0x00 | 6,
	ios_port_0_pin_7  = 0x00 | 7,
	ios_port_0_pin_8  = 0x00 | 8,
	ios_port_0_pin_9  = 0x00 | 9,
	ios_port_0_pin_10 = 0x00 | 10,
	ios_port_0_pin_11 = 0x00 | 11,
	ios_port_0_pin_12 = 0x00 | 12,
	ios_port_0_pin_13 = 0x00 | 13,
	ios_port_0_pin_14 = 0x00 | 14,
	ios_port_0_pin_15 = 0x00 | 15,
	ios_port_0_pin_16 = 0x00 | 16,
	ios_port_0_pin_17 = 0x00 | 17,
	ios_port_0_pin_18 = 0x00 | 18,
	ios_port_0_pin_19 = 0x00 | 19,
	ios_port_0_pin_20 = 0x00 | 20,
	ios_port_0_pin_21 = 0x00 | 21,
	ios_port_0_pin_22 = 0x00 | 22,
	ios_port_0_pin_23 = 0x00 | 23,
	ios_port_0_pin_24 = 0x00 | 24,
	ios_port_0_pin_25 = 0x00 | 25,
	ios_port_0_pin_26 = 0x00 | 26,
	ios_port_0_pin_27 = 0x00 | 27,
	ios_port_0_pin_28 = 0x00 | 28,
	ios_port_0_pin_29 = 0x00 | 29,
	ios_port_0_pin_30 = 0x00 | 30,
	ios_port_0_pin_31 = 0x00 | 31,
	ios_port_0_pin_32 = 0x00 | 32,

	ios_port_1_pin_0  = 0x40 | 0,
	ios_port_1_pin_1  = 0x40 | 1,
	ios_port_1_pin_2  = 0x40 | 2,
	ios_port_1_pin_3  = 0x40 | 3,
	ios_port_1_pin_4  = 0x40 | 4,
	ios_port_1_pin_5  = 0x40 | 5,
	ios_port_1_pin_6  = 0x40 | 6,
	ios_port_1_pin_7  = 0x40 | 7,
	ios_port_1_pin_8  = 0x40 | 8,
	ios_port_1_pin_9  = 0x40 | 9,
	ios_port_1_pin_10 = 0x40 | 10,
	ios_port_1_pin_11 = 0x40 | 11,
	ios_port_1_pin_12 = 0x40 | 12,
	ios_port_1_pin_13 = 0x40 | 13,
	ios_port_1_pin_14 = 0x40 | 14,
	ios_port_1_pin_15 = 0x40 | 15,
	ios_port_1_pin_16 = 0x40 | 16,
	ios_port_1_pin_17 = 0x40 | 17,
	ios_port_1_pin_18 = 0x40 | 18,
	ios_port_1_pin_19 = 0x40 | 19,
	ios_port_1_pin_20 = 0x40 | 20,
	ios_port_1_pin_21 = 0x40 | 21,
	ios_port_1_pin_22 = 0x40 | 22,
	ios_port_1_pin_23 = 0x40 | 23,
	ios_port_1_pin_24 = 0x40 | 24,
	ios_port_1_pin_25 = 0x40 | 25,
	ios_port_1_pin_26 = 0x40 | 26,
	ios_port_1_pin_27 = 0x40 | 27,
	ios_port_1_pin_28 = 0x40 | 28,
	ios_port_1_pin_29 = 0x40 | 29,
	ios_port_1_pin_30 = 0x40 | 30,
	ios_port_1_pin_31 = 0x40 | 31,
	ios_port_1_pin_32 = 0x40 | 32,

	ios_owts_data = ios_port_1_pin_28,
	ios_owts_debug = ios_port_1_pin_27,

	ios_dpy_col_3 = ios_port_1_pin_20,
	ios_dpy_col_2 = ios_port_0_pin_18,
	ios_dpy_col_1 = ios_port_1_pin_29,
	ios_dpy_col_0 = ios_port_1_pin_31,
	ios_dpy_col_7 = ios_port_0_pin_14,
	ios_dpy_col_6 = ios_port_0_pin_17,
	ios_dpy_col_5 = ios_port_0_pin_3,
	ios_dpy_col_4 = ios_port_1_pin_15,

	ios_dpy_line_green_0 = ios_port_0_pin_12,
	ios_dpy_line_green_1 = ios_port_0_pin_5,
	ios_dpy_line_green_2 = ios_port_0_pin_20,
	ios_dpy_line_green_3 = ios_port_1_pin_13,
	ios_dpy_line_green_4 = ios_port_0_pin_11,
	ios_dpy_line_green_5 = ios_port_1_pin_19,
	ios_dpy_line_green_6 = ios_port_0_pin_19,
	ios_dpy_line_green_7 = ios_port_0_pin_8,

	ios_dpy_line_red_0 = ios_port_0_pin_13,
	ios_dpy_line_red_1 = ios_port_0_pin_16,
	ios_dpy_line_red_2 = ios_port_0_pin_6,
	ios_dpy_line_red_3 = ios_port_1_pin_14,
	ios_dpy_line_red_4 = ios_port_1_pin_21,
	ios_dpy_line_red_5 = ios_port_0_pin_7,
	ios_dpy_line_red_6 = ios_port_0_pin_2,
	ios_dpy_line_red_7 = ios_port_1_pin_16,

	ios_dpy_col_total = 8,
	ios_dpy_line_total = 8, 
	ios_dpy_groups_total = 3
};

void ios_set (enum ios_pin, bool value);
bool ios_get (enum ios_pin);

void ios_set_output (enum ios_pin);
void ios_set_input (enum ios_pin);

static inline uint8_t ios_port_number (enum ios_pin portpin)
{
	return portpin >> 6;
}
static inline uint8_t ios_pin_number (enum ios_pin portpin)
{
	return portpin & 0x3F;
}

#endif /* SRC_IOS_H_ */
