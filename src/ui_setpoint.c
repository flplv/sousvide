#include "ui.h"
#include "ui_internal.h"
#include "control.h"
#include "display.h"

static int16_t selected_setpoing;

static void input_temperature (int16_t temperature)
{
}

static void input_action (struct ui_input_action action)
{
    int sp = selected_setpoing;
    sp += action.intensity * (action.type == ui_action_left ? -1 : +1);

    if (sp < 0)
        sp = 0;

    if (sp > 99)
        sp = 99;

    selected_setpoing = sp;
    display_draw_temperature (selected_setpoing);
}

static void on_enter ()
{
    selected_setpoing = control_get_setpoint ();

    display_draw_temperature_color (pixel_orange);
    display_draw_temperature (selected_setpoing);
    display_draw_clock (-1);
    display_draw_underline (pixel_off);
}

static void on_one_sec_ticker ()
{
//    static bool s = 0;
//
//    if (s)
//        display_draw_underline (pixel_green);
//    else
//        display_draw_underline (pixel_off);
//
//    s = !s;
}

static void on_exit ()
{
    control_set_setpoint (selected_setpoing);
}

const struct ui_processor ui_setpoint_processor =
{
    .input_temperature = input_temperature,
    .input_action = input_action,
    .on_enter = on_enter,
    .on_exit = on_exit,
    .on_one_sec_ticker = on_one_sec_ticker
};
