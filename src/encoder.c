#include <reacto/reusables/timeout.h>
#include "ios.h"
#include "encoder.h"
#include "ui.h"

void encoder_input_event (enum ios_input_event event, enum ios_pin pin)
{
	static reacto_time_t last = 0;
	reacto_time_t now = time_now ();

	if (!timeout_check_elapsed(now, last, 20))
		return;

	last = now;

	if (pin == ios_encoder_switch)
	{
		ui_push (event == ios_input_event_down ? ui_event_click_down : ui_event_click_up);
		return;
	}

	if (event == ios_input_event_down)
	{
		if (pin == ios_encoder_left)
			ui_push (ui_event_left);

		if (pin == ios_encoder_right)
			ui_push (ui_event_right);
	}
}

void encoder_init ()
{
    ios_set_input (ios_encoder_left);
    ios_set_input (ios_encoder_right);
    ios_set_input (ios_encoder_switch);
}

