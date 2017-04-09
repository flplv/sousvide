#include "ui.h"
#include "ui_internal.h"
#include "control.h"
#include "display.h"

static int16_t selected_timer;

static void input_temperature (int16_t temperature)
{
}

static void input_action (struct ui_input_action action)
{
    int16_t timer = selected_timer;
    timer += 10 * action.intensity * (action.type == ui_action_left ? -1 : +1);

    if (timer > 400)
        timer = 400;

    if (timer < 0)
        timer = -10;

    timer -= timer % 10;

    selected_timer = timer;
    display_draw_clock (timer);
}

static void on_enter ()
{
    selected_timer = control_get_timer ();

    display_draw_temperature_color (pixel_orange);
    display_draw_timer_symbol ();
    display_draw_clock (selected_timer);
    display_draw_underline (pixel_off);
}

static void on_one_sec_ticker ()
{
//    static bool s = 0;
//
//    if (s)
//        display_draw_underline (pixel_red);
//    else
//        display_draw_underline (pixel_off);
//
//    s = !s;
}

static void on_exit ()
{
    control_set_timer (selected_timer);
}

const struct ui_processor ui_timer_processor =
{
    .input_temperature = input_temperature,
    .input_action = input_action,
    .on_enter = on_enter,
    .on_exit = on_exit,
    .on_one_sec_ticker = on_one_sec_ticker
};
