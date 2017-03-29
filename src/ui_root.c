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
    int16_t timer = control_get_timer ();
    timer += 10 * action.intensity * (action.type == ui_action_left ? -1 : +1);

    if (timer < 0)
        timer = -1;

    if (timer > 400)
        timer = 400;

    timer -= timer % 10;

    control_set_timer (timer);
    display_draw_clock (timer);
}

static void on_enter ()
{
    display_draw_temperature_color (pixel_red);
    display_draw_temperature (current_temperature);
    display_draw_clock (control_get_timer ());
    display_draw_underline (pixel_off);
}

static void on_one_sec_ticker ()
{
    display_tick_clock (control_get_timer ());
}

const struct ui_processor ui_root_processor =
{
    .input_temperature = input_temperature,
    .input_action = input_action,
    .on_enter = on_enter,
    .on_one_sec_ticker = on_one_sec_ticker
};
