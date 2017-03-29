#include "reacto_factory_config.h"
#include <reacto/context.h>
#include <reacto/reusables/time.h>
#include <reacto/reusables/timeout.h>
#include "ui.h"
#include "ui_internal.h"
#include "events.h"
#include "display.h"
#include "fl-lib.h"

struct event
{
    enum ui_event type;
    reacto_time_t timestamp;
};

static struct event ui_event_buffer [ui_buffer_size];
static timed_event_t one_sec_ev;
static timed_event_t timeout_ev;

static const struct ui_processor * registered_uis [] =
{
        &ui_root_processor,
        &ui_setpoint_processor,
        NULL
};

static int selected_ui = 0;

static void on_one_sec (timed_event_t * ev)
{
    const struct ui_processor * ui = registered_uis[selected_ui];
    ui->on_one_sec_ticker ();

    /* Relink to get called cyclicly */
    timed_queue_link (reacto_context_timed_queue (), ev);
}


static void timeout (timed_event_t * ev)
{
    if (selected_ui == 0)
        return;

    selected_ui = 0;
    registered_uis [selected_ui]->on_enter ();
}

static void consume_input (struct event ev)
{
    static reacto_time_t last_scroll = 0;
    static enum ui_event last_scroll_direction;
    reacto_time_t now = time_now ();

    if (ev.type == ui_event_click_down)
    {
        selected_ui ++;
        selected_ui %= FL_ARRAY_SIZE(registered_uis) - 1;

        registered_uis [selected_ui]->on_enter ();
    }
    else
    {   struct ui_input_action action;

        if (last_scroll_direction == ev.type)
            action.intensity = timeout_check_elapsed (now, last_scroll, 20) ? 1 : 5;
        else
            action.intensity = 1;

        action.type = ev.type == ui_event_left ? ui_action_left : ui_action_right;

        registered_uis [selected_ui]->input_action (action);

        last_scroll_direction = ev.type;
        last_scroll = now;
    }

    timed_queue_link (reacto_context_timed_queue(), &timeout_ev);
}

int ui_handler (queue_t * queue)
{
    struct event ev = {0,};
    queue_peek (reacto_context_queue(reacto_queue_id_ui), ui_event_buffer, &ev);

    consume_input (ev);
    return 0;
}

void ui_push (enum ui_event action)
{
    struct event ev;
    ev.type = action;
    ev.timestamp = time_now ();

    queue_push (reacto_context_queue(reacto_queue_id_ui), ui_event_buffer, ev);
}

void ui_temperature (int16_t temperature)
{
    const struct ui_processor ** ui = registered_uis;

    while (*ui)
    {
        (*ui)->input_temperature (temperature);
        ui ++;
    }
}

const struct ui_processor * ui_get_current_processor ()
{
    return registered_uis[selected_ui];
}

void ui_init ()
{
    timed_event_init (&one_sec_ev, 1000, on_one_sec);
    timed_queue_link (reacto_context_timed_queue(), &one_sec_ev);

    timed_event_init (&timeout_ev, 5000, timeout);

    selected_ui = 0;
    registered_uis[selected_ui]->on_enter ();
}
