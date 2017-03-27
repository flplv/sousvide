#include <ios.h>
#include <chip.h>
#include <fl-lib.h>

void ios_set (enum ios_pin pp, bool value)
{
	Chip_GPIO_SetPinState(LPC_GPIO,
			ios_port_number (pp),
			              ios_pin_number (pp),
			              value);
}

bool ios_get (enum ios_pin pp)
{
	return Chip_GPIO_GetPinState (LPC_GPIO,
			              ios_port_number (pp),
			              ios_pin_number (pp));
}

void ios_set_output (enum ios_pin pp)
{
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,
			ios_port_number (pp),
            ios_pin_number (pp));
}

void ios_set_input (enum ios_pin pp)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,
			ios_port_number (pp),
            ios_pin_number (pp));
}

struct pin_mux_cfg
{
	enum ios_pin pin;
	uint8_t mux;
};

static const struct pin_mux_cfg pin_mux_mapping [] = {

		/* dpy */
		{ios_port_0_pin_11, IOCON_FUNC1},
		{ios_port_0_pin_12, IOCON_FUNC1},
		{ios_port_0_pin_13, IOCON_FUNC1},
		{ios_port_0_pin_14, IOCON_FUNC1},
		{ios_port_0_pin_16, IOCON_FUNC0},
		{ios_port_0_pin_17, IOCON_FUNC0},
		{ios_port_0_pin_18, IOCON_FUNC0},
		{ios_port_0_pin_19, IOCON_FUNC0},
		{ios_port_0_pin_2 , IOCON_FUNC0},
		{ios_port_0_pin_20, IOCON_FUNC0},
		{ios_port_0_pin_3 , IOCON_FUNC0},
		{ios_port_0_pin_5 , IOCON_FUNC0},
		{ios_port_0_pin_6 , IOCON_FUNC0},
		{ios_port_0_pin_7 , IOCON_FUNC0},
		{ios_port_0_pin_8 , IOCON_FUNC0},
		{ios_port_1_pin_13, IOCON_FUNC0},
		{ios_port_1_pin_14, IOCON_FUNC0},
		{ios_port_1_pin_15, IOCON_FUNC0},
		{ios_port_1_pin_16, IOCON_FUNC0},
		{ios_port_1_pin_19, IOCON_FUNC0},
		{ios_port_1_pin_20, IOCON_FUNC0},
		{ios_port_1_pin_21, IOCON_FUNC0},
		{ios_port_1_pin_29, IOCON_FUNC0},
		{ios_port_1_pin_31, IOCON_FUNC0},

		/* owts */
		{ios_port_1_pin_28, IOCON_FUNC0},
		{ios_port_1_pin_27, IOCON_FUNC0}
};

void ios_platform_init ()
{
	Chip_GPIO_Init(LPC_GPIO);

	/* pin mux */

	for (int cfg = 0; cfg < FL_ARRAY_SIZE (pin_mux_mapping); cfg++)
	{
		uint8_t pin = ios_pin_number (pin_mux_mapping [cfg].pin);
		uint8_t port = ios_port_number (pin_mux_mapping [cfg].pin);
		uint8_t mux = pin_mux_mapping [cfg].mux;

		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, mux);
	}
}
