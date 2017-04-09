#include "reacto_factory_config.h"
#include <reacto/context.h>
#include <reacto/reusables/timeout.h>
#include "control.h"
#include "owts.h"
#include "ios.h"
#include "ui.h"

#define histeresis 8

static int32_t temperature_buffer [control_buffer_size];
static int16_t setpoint_temperature = 600;
static int16_t timer_minutes = -1;
static reacto_time_t last_control_ms = 0;
static bool state;

static void off ()
{
    ios_set (ios_control_relay_1, 1);
    ios_set (ios_control_relay_2, 1);
    state = false;
}

static void on ()
{
    ios_set (ios_control_relay_1, 0);
    ios_set (ios_control_relay_2, 0);
    state = true;
}

static void control (int16_t current_temperature)
{
    reacto_time_t now = time_now ();
    if (timeout_check_elapsed (now, last_control_ms, 60 * 1000))
    {
        timer_minutes --;
        last_control_ms = now;
    }

    if (timer_minutes < 0)
    {
        timer_minutes = -1;
        off ();
    }
    else
    {
        if (current_temperature < (setpoint_temperature - (histeresis/2)))
            on ();
        else if (current_temperature > (setpoint_temperature + (histeresis/2)))
            off ();
    }
}

int control_handler (queue_t * queue)
{
    int32_t temperature = 0;
    queue_peek(queue, temperature_buffer, &temperature);

    ui_temperature (temperature / 1000);
    control (temperature / 100);
    owts_start_conversion ();
    return 0;
}

int16_t control_get_setpoint ()
{
    return setpoint_temperature / 10;
}

void control_set_setpoint (int16_t sp)
{
    setpoint_temperature = sp * 10;
}

int16_t control_get_timer ()
{
    return timer_minutes;
}

void control_set_timer (int16_t timer)
{
    timer_minutes = timer;
}

bool control_is_on ()
{
    return state;
}

void control_init ()
{
    off ();
    ios_set_output (ios_control_relay_1);
    ios_set_output (ios_control_relay_2);

    last_control_ms = time_now ();
    owts_start_conversion ();
}

void owts_on_temperature (enum conversion_result result, int32_t temperature)
{
//    int32_t integer = temperature / 1000;
//    int32_t decimal = temperature - (integer * 1000);
    queue_push(reacto_context_queue(reacto_queue_id_control), temperature_buffer, temperature);
}

