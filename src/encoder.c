#include <reacto/reusables/timeout.h>
#include "ios.h"
#include "encoder.h"
#include "ui.h"

void encoder_input_scroll (bool dt_state)
{
	static reacto_time_t last = 0;
	reacto_time_t now = time_now ();

	if (!timeout_check_elapsed(now, last, 10))
		return;

	last = now;

    ios_set (ios_encoder_debug, 0);
    if (dt_state)
        ui_push (ui_event_left);
    else
        ui_push (ui_event_right);
    ios_set (ios_encoder_debug, 1);
}

void encoder_input_push ()
{
	static reacto_time_t last = 0;
	reacto_time_t now = time_now ();

	if (!timeout_check_elapsed(now, last, 500))
		return;

    ui_push (ui_event_click_down);
}

void encoder_init ()
{
    ios_set_input (ios_encoder_clk);
    ios_set_input (ios_encoder_dt);
    ios_set_input (ios_encoder_switch);

    ios_set (ios_encoder_debug, 1);
    ios_set_output (ios_encoder_debug);
}

