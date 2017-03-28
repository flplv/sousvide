#include "reacto_factory_config.h"
#include <reacto/context.h>
#include <reacto/reusables/time.h>
#include "ui.h"
#include "events.h"

struct event
{
    enum ui_event type;
    reacto_time_t timestamp;
};

static struct event ui_event_buffer [ui_buffer_size];
static timed_event_t one_sec_ev;
static timed_event_t timeout_ev;

static void on_one_sec (timed_event_t * ev)
{
    display_tick_clock(120);

    /* Relink to get called cyclicly */
    timed_queue_link(reacto_context_timed_queue(), ev);
}

int ui_handler (queue_t * queue)
{
    struct event ev;
    queue_peek (reacto_context_queue(reacto_queue_id_ui), ui_event_buffer, &ev);

    /* TODO Do something with user input */
    log_message ("User input %d", ev.type);
    return 0;
}

void ui_push (enum ui_event action)
{
    struct event ev;
    ev.type = action;
    ev.timestamp = time_now ();

    queue_push(reacto_context_queue(reacto_queue_id_ui), ui_event_buffer, ev);
}

void ui_temperature (int16_t temperature)
{
    display_set_temperature (temperature / 1000);
}

void ui_init ()
{
    timed_event_init(&one_sec_ev, 1000, on_one_sec);
    timed_queue_link(reacto_context_timed_queue(), &one_sec_ev);
}
