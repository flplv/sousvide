#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>
#include <reacto/queue.h>
#include <reacto/reusables/log.h>
#include "events.h"
#include "error.h"
#include "owts.h"

#define queue_temperature 0
#define queue_log 1

#define temperature_buffer_size 8
static int32_t temperature_buffer [temperature_buffer_size];

#define log_buffer_size 1024
static char log_buffer [log_buffer_size];

static int temperature_handler (queue_t * queue)
{
	int32_t temperature;
    queue_peek(queue, temperature_buffer, &temperature);

    log_message ("T=%"PRIi32, temperature);
	display_set_temperature (temperature / 1000);
	owts_start_conversion ();
	return 0;
}

void error (enum errors err)
{
	log_message ("Error %d", err);
}

void owts_on_temperature (enum conversion_result result, int32_t temperature)
{
//	int32_t integer = temperature / 1000;
//	int32_t decimal = temperature - (integer * 1000);
	queue_push(reacto_context_queue(queue_temperature), temperature_buffer, temperature);
}

static int log_handler (queue_t * queue)
{
    char c;
    queue_peek(queue, log_buffer, &c);
    putchar (c);
    return 0;
}

static void log_string (const char * str)
{
	while (*str)
		queue_push(reacto_context_queue(queue_log), log_buffer, *str++);
	queue_push(reacto_context_queue(queue_log), log_buffer, '\n');
}

static void clock_tick_handler (timed_event_t * ev)
{
	display_tick_clock(120);

    /* Relink to get called cyclicly */
    timed_queue_link(reacto_context_timed_queue(), ev);
}

void log_message (const char * format, ...)
{
	static char buf [256];

    va_list ap;
    va_start(ap, format);

	vsnprintf(buf, 255, format, ap);
	buf[255] = '\0';

	log_string (buf);

	va_end(ap);
}

struct reacto_queues_context queues_context = {

    /* Installing the queue handlers */
    .queue_handlers = {
        /* Queues handlers for the first queue */
        [queue_temperature] = {
			temperature_handler
        },
        [queue_log] = {
			log_handler
        },
    },
    .queue_buffer_sizes = {
        [queue_temperature] = temperature_buffer_size,
        [queue_log] = log_buffer_size,
    },
};

const struct reacto_timed_queue_context timed_queue_context =
{
    .high_priority = true,
};

void events_init ()
{
	static timed_event_t ev;

    timed_event_init(&ev, 1000, clock_tick_handler);
    timed_queue_link(reacto_context_timed_queue(), &ev);
    owts_start_conversion ();
}
