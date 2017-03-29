#include <stdio.h>
#include <inttypes.h>
#include "reacto_factory_config.h"
#include <reacto/context_factory.template>
#include <reacto/queue.h>
#include <reacto/reusables/log.h>
#include "events.h"
#include "owts.h"
#include "ui.h"
#include "logging.h"

#define temperature_buffer_size 8
static int32_t temperature_buffer [temperature_buffer_size];

static int temperature_handler (queue_t * queue)
{
    int32_t temperature = 0;
    queue_peek(queue, temperature_buffer, &temperature);

    ui_temperature (temperature / 1000);
    owts_start_conversion ();
    return 0;
}

void owts_on_temperature (enum conversion_result result, int32_t temperature)
{
//    int32_t integer = temperature / 1000;
//    int32_t decimal = temperature - (integer * 1000);
    queue_push(reacto_context_queue(reacto_queue_id_temperature), temperature_buffer, temperature);
}


static struct reacto_queues_context queues_context = {

    /* Installing the queue handlers */
    .queue_handlers = {
        /* Queues handlers for the first queue */
        [reacto_queue_id_temperature] = {
            temperature_handler
        },
        [reacto_queue_id_ui] = {
            ui_handler
        },
        [reacto_queue_id_log] = {
            log_handler
        },
    },
    .queue_buffer_sizes = {
        [reacto_queue_id_temperature] = temperature_buffer_size,
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
