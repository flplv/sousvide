#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>
#include "reacto_factory_config.h"
#include <reacto/context.h>
#include "logging.h"
#include "error.h"

static char log_buffer [log_buffer_size];

static void log_string (const char * str)
{
    while (*str)
        queue_push(reacto_context_queue(reacto_queue_id_log), log_buffer, *str++);
    queue_push(reacto_context_queue(reacto_queue_id_log), log_buffer, '\n');
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

int log_handler (queue_t * queue)
{
    char c;
    queue_peek(queue, log_buffer, &c);
    putchar (c);
    return 0;
}

void error (enum errors err)
{
    log_message ("Error %d", err);
}
