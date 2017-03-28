#include <ios.h>
#include <chip.h>
#include <fl-lib.h>
#include <encoder.h>
#include <priorities.h>

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
        {ios_port_1_pin_27, IOCON_FUNC0},

        /* encoder */
        {ios_port_1_pin_26, IOCON_FUNC0 | IOCON_MODE_PULLUP},
        {ios_port_1_pin_24, IOCON_FUNC0 | IOCON_MODE_PULLUP},
        {ios_port_1_pin_25, IOCON_FUNC0 | IOCON_MODE_PULLUP},
};

static inline void process_encoder_pin_interrupt (enum ios_pin pin, uint8_t ch)
{
    uint32_t fall = Chip_PININT_GetFallStates (LPC_PININT) & PININTCH (ch);
	Chip_PININT_ClearFallStates (LPC_PININT, fall);
    Chip_PININT_ClearIntStatus (LPC_PININT, PININTCH (ch));

    if (fall)
        encoder_input_event (ios_input_event_down, pin);
}

void FLEX_INT0_IRQHandler ()
{
    process_encoder_pin_interrupt (ios_encoder_left, 0);
    NVIC_ClearPendingIRQ(PIN_INT0_IRQn);
}

void FLEX_INT1_IRQHandler ()
{
    process_encoder_pin_interrupt (ios_encoder_right, 1);
    NVIC_ClearPendingIRQ(PIN_INT1_IRQn);
}

void FLEX_INT2_IRQHandler ()
{
    process_encoder_pin_interrupt (ios_encoder_switch, 2);
    NVIC_ClearPendingIRQ(PIN_INT2_IRQn);
}

void ios_platform_init ()
{
    Chip_GPIO_Init (LPC_GPIO);
    Chip_Clock_EnablePeriphClock (SYSCTL_CLOCK_PINT);
    Chip_PININT_Init (LPC_PININT);

    Chip_SYSCTL_SetPinInterrupt (0,
                                 ios_port_number(ios_encoder_left),
                                 ios_pin_number(ios_encoder_left));

    Chip_SYSCTL_SetPinInterrupt (1,
                                 ios_port_number(ios_encoder_right),
                                 ios_pin_number(ios_encoder_right));

    Chip_SYSCTL_SetPinInterrupt (2,
                                 ios_port_number(ios_encoder_switch),
                                 ios_pin_number(ios_encoder_switch));

    Chip_PININT_SetPinModeEdge (LPC_PININT, 0x03);
    Chip_PININT_EnableIntLow (LPC_PININT, 0x03);

    NVIC_EnableIRQ(PIN_INT0_IRQn);
    NVIC_EnableIRQ(PIN_INT1_IRQn);
    NVIC_EnableIRQ(PIN_INT2_IRQn);
    NVIC_SetPriority (PIN_INT0_IRQn, priorities_encoder);
    NVIC_SetPriority (PIN_INT1_IRQn, priorities_encoder);
    NVIC_SetPriority (PIN_INT2_IRQn, priorities_encoder);

    /* pin mux */

    for (int cfg = 0; cfg < FL_ARRAY_SIZE (pin_mux_mapping); cfg++)
    {
        uint8_t pin = ios_pin_number (pin_mux_mapping [cfg].pin);
        uint8_t port = ios_port_number (pin_mux_mapping [cfg].pin);
        uint8_t mux = pin_mux_mapping [cfg].mux;

        Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, mux);
    }
}
