#include <stdio.h>
#include "reacto_factory_config.h"
#include <reacto/context_factory.template>
#include "events.h"
#include "ui.h"
#include "logging.h"
#include "control.h"

static struct reacto_queues_context queues_context = {

    /* Installing the queue handlers */
    .queue_handlers = {
        /* Queues handlers for the first queue */
        [reacto_queue_id_control] = {
            control_handler
        },
        [reacto_queue_id_ui] = {
            ui_handler
        },
        [reacto_queue_id_log] = {
            log_handler
        },
    },
    .queue_buffer_sizes = {
        [reacto_queue_id_control] = control_buffer_size,
        [reacto_queue_id_ui] = ui_buffer_size,
        [reacto_queue_id_log] = log_buffer_size,
    },
};

static const struct reacto_timed_queue_context timed_queue_context =
{
    .high_priority = true,
};

void events_init ()
{
    int r = reacto_context_factory(main_loop_strategy_priority_queue,
                                   &timed_queue_context,
                                   &queues_context);
    if (r < 0)
    {
        printf ("reacto_context_factory failed with %d.", r);
        return;
    }
}
