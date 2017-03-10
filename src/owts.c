/* One Wire Temperature Sensor Module
   chip: DS18B20
   line procedure method: 1 us ISR
   line drive method: open drain
   zero: 30 us
   one: 1 us
   read pull: 2 us
   aqcuire timeout: 12 us
   read time slot: 60 us
   recovery: 1 us
 */

#include <fl-lib.h>
#include "owts.h"
#include "error.h"

static const uint16_t period_reset = 480;
static const uint16_t period_tx_presence = 70;

struct event {
    /* enum owts_action */ uint8_t action;
    uint16_t duration;
    int (*consumer) (bool);
};

struct owts {
    struct event fifo_buffer [128]; /* Power of two required */
    fl_ring_fifo_t fifo;
};
static struct owts d;

static int consume_presence (bool state)
{
    if (state == 0)
        return 0;

    error (error_owts_no_presence);
    return -1;
}

static void push (const struct event * e)
{
    if (fl_ring_fifo_full (&d.fifo))
    {
        /* error */
        return;
    }

    d.fifo_buffer[fl_ring_fifo_write_index(&d.fifo)] = *e;
    fl_ring_fifo_write_increment(&d.fifo);
}

#define PUSH(__a,__b,__c) e.action=__a; e.duration=__b; e.consumer=__c; push(&e)

static void pulse_initialize ()
{
    struct event e;
    PUSH (owts_wire_pull, period_reset, NULL);
    PUSH (owts_wire_release, period_tx_presence, NULL);
    PUSH (owts_wire_acquire, period_reset, consume_presence);
}

static void owts_on_timeout (uint16_t timestamp)
{
    if (fl_ring_fifo_empty(&d.fifo))
        return;

    struct event * e = d.fifo_buffer + fl_ring_fifo_read_index(&d.fifo);

    bool line_state = owts_drive_line ((enum owts_action)e->action);
    bool abort = false;

    if (e->consumer && e->action == owts_wire_acquire)
        if (e->consumer (line_state) < 0)
            abort = true;

    if (abort)
    {
        fl_ring_fifo_init (&d.fifo, FL_ARRAY_SIZE (d.fifo_buffer));
        return;
    }

    fl_ring_fifo_read_increment(&d.fifo);
    owts_isr_register_call (owts_on_timeout, timestamp + e->duration);
}


void owts_init ()
{
    fl_ring_fifo_init (&d.fifo, FL_ARRAY_SIZE (d.fifo_buffer));
}

void owts_deinit ()
{
}
