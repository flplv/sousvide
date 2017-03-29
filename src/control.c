#include "control.h"

int16_t setpoint_temperature = 0;
int16_t timer_minutes = 0;

static void on ()
{

}

static void off ()
{

}

int16_t control_get_setpoint ()
{
    return setpoint_temperature;
}

void control_set_setpoint (int16_t sp)
{
    setpoint_temperature = sp;
}

int16_t control_get_timer ()
{
    return timer_minutes;
}

void control_set_timer (int16_t timer)
{
    timer_minutes = timer;
}

void control_process (int16_t current_temperature)
{
    if (timer_minutes < 0)
        off ();

    else
        if (current_temperature < setpoint_temperature)
            on ();
        else
            off ();

}
