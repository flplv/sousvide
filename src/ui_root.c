#include "ui.h"
#include "ui_internal.h"
#include "control.h"
#include "display.h"

static int16_t current_temperature = -1;

static void input_temperature (int16_t temperature)
{
    current_temperature = temperature;

    if (ui_get_current_processor () == &ui_root_processor)
        display_draw_temperature (temperature);
}

static void input_action (struct ui_input_action action)
{
}

static void on_enter ()
{
    display_draw_temperature_color (pixel_red);
    display_draw_temperature (current_temperature);
    display_draw_clock (control_get_timer ());
    display_draw_underline (pixel_off);
}

static void on_exit ()
{
}

static void on_one_sec_ticker ()
{
    static bool s = 0;

    display_tick_clock (control_get_timer ());

    if (s)
        display_draw_underline (control_is_on () ? pixel_orange : pixel_off);
    else
        display_draw_underline (pixel_off);

    s = !s;
}

const struct ui_processor ui_root_processor =
{
    .input_temperature = input_temperature,
    .input_action = input_action,
    .on_enter = on_enter,
    .on_exit = on_exit,
    .on_one_sec_ticker = on_one_sec_ticker
};
