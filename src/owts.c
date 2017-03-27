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
#include "ios.h"

static const uint16_t period_reset = 480;
static const uint16_t period_tx_presence = 40;
static const uint16_t period_write_zero = 80;
static const uint16_t period_line_settle_zero = 20;
static const uint16_t period_write_one = 0;
static const uint16_t period_line_settle_one = 100;
static const uint16_t period_pull_aquire = 1;
static const uint16_t period_aquire_timeout = 0;
static const uint16_t period_line_settle_aquire = 80;
static const uint16_t period_line_settle_aquire_wait_conversion = 10000;

enum consume_result {
    owts_ok,
    owts_abort,
    owts_try_again,
};

struct event {
    enum owts_action  /*uint8_t*/ action;
    uint16_t duration;
    enum consume_result (*consumer) (bool);
};

struct owts {
    struct event event_buffer [128];
    uint8_t event_buffer_read_pos;
    uint8_t event_buffer_write_pos;
    int16_t raw_temperature;
    uint8_t raw_temperature_bit_pos;
	enum conversion_result conversion_result;
};
static struct owts d;

static inline bool owts_drive_line (enum owts_action action)
{
	ios_set (ios_owts_debug, 0);

    if (action == owts_wire_pull)
    {
    	ios_set_output (ios_owts_data);
    }
    else
    {
    	ios_set_input (ios_owts_data);
    }

    bool s = ios_get (ios_owts_data);
    ios_set (ios_owts_debug, 1);

    return s;
}

static inline struct event * event_buffer_peek ()
{
    return &d.event_buffer[d.event_buffer_read_pos];
}

static inline void event_buffer_pop ()
{
    d.event_buffer_read_pos++;
}

static void event_buffer_push (const struct event * e)
{
    d.event_buffer[d.event_buffer_write_pos++] = *e;
}

static void event_buffer_rewind ()
{
    d.event_buffer_write_pos = 0;
    d.event_buffer_read_pos = 0;
}

static inline void event_buffer_recovery (int amount)
{
    d.event_buffer_read_pos -= amount;
}

static inline bool event_buffer_empty ()
{
    return d.event_buffer_read_pos == d.event_buffer_write_pos;
}

static inline bool event_buffer_full ()
{
    return d.event_buffer_write_pos == FL_ARRAY_SIZE (d.event_buffer);
}

static enum consume_result consume_presence (bool state)
{
    if (state == 0)
        return owts_ok;

    d.conversion_result = owts_conversion_no_presence;
    return owts_abort;
}

static enum consume_result consume_wait_conversion (bool state)
{
    if (state == 1)
        return owts_ok;

    return owts_try_again;
}

static enum consume_result consume_bit (bool state)
{
    d.raw_temperature |= (state ? 1 : 0) << d.raw_temperature_bit_pos ++;
    return owts_ok;
}

static void push (const struct event * e)
{
    if (event_buffer_full ())
    {
        error (error_owts_event_buffer_too_small);
        return;
    }

    event_buffer_push (e);
}

#define PUSH(__a,__b,__c) e.action=__a; e.duration=__b; e.consumer=__c; push(&e)

static void pulse_initialize ()
{
    struct event e;
    PUSH (owts_wire_pull, period_reset, NULL);
    PUSH (owts_wire_release, period_tx_presence, NULL);
    PUSH (owts_wire_acquire, period_reset, consume_presence);
}

static void pulse_one ()
{
    struct event e;
    PUSH (owts_wire_pull, period_write_one, NULL);
    PUSH (owts_wire_release, period_line_settle_one, NULL);
}

static void pulse_zero ()
{
    struct event e;
    PUSH (owts_wire_pull, period_write_zero, NULL);
    PUSH (owts_wire_release, period_line_settle_zero, NULL);
}

static void pulse_acquire ()
{
    struct event e;
    PUSH (owts_wire_pull, period_pull_aquire, NULL);
    PUSH (owts_wire_release, period_aquire_timeout, NULL);
    PUSH (owts_wire_acquire, period_line_settle_aquire, consume_bit);
}

static void pulse_wait_conversion ()
{
    struct event e;
    PUSH (owts_wire_pull, period_pull_aquire, NULL);
    PUSH (owts_wire_release, period_aquire_timeout, NULL);
    PUSH (owts_wire_acquire, period_line_settle_aquire_wait_conversion, consume_wait_conversion);
}

static void pulse_byte (uint8_t byte)
{
    int n = 8;
    while (n--)
    {
        const bool bit = byte & 0x1;
        byte >>= 1;
        if (bit)
            pulse_one ();
        else
            pulse_zero ();
    }
}

static void pulse_read_byte ()
{
    int n = 8;
    while (n--)
        pulse_acquire ();
}

static void owts_on_timeout (uint16_t timestamp)
{
    struct event * e = event_buffer_peek ();
    event_buffer_pop ();

    bool line_state = owts_drive_line ((enum owts_action)e->action);

    enum consume_result result = owts_ok;
    if (e->consumer && e->action == owts_wire_acquire)
        result = e->consumer (line_state);

    if (result == owts_abort)
    {
    	enum conversion_result result = d.conversion_result;
        owts_init ();

        owts_on_temperature (result, 0);
        return;
    }

    if (result == owts_try_again)
        event_buffer_recovery (3);

    if (event_buffer_empty ())
    {
        /* end of transmission */

    	if (d.raw_temperature_bit_pos == 16)
        	d.conversion_result = owts_conversion_success;

    	enum conversion_result result = d.conversion_result;
    	int32_t temp = ((int16_t)d.raw_temperature >> 1) * 125;

        owts_init ();
        owts_isr_stop ();
        owts_on_temperature (result, temp);
        return;
    }

    owts_isr_next_call (timestamp + e->duration);
}

void owts_start_conversion ()
{
    pulse_initialize ();
    pulse_byte (0xCC);
    pulse_byte (0x44);
    pulse_wait_conversion ();
    pulse_initialize ();
    pulse_byte (0xCC);
    pulse_byte (0xBE);
    pulse_read_byte ();
    pulse_read_byte ();
    pulse_initialize ();
    owts_isr_next_call (1000);
    owts_isr_start (owts_on_timeout);
}

void owts_init ()
{
    event_buffer_rewind ();
    d.conversion_result = owts_conversion_fifo_unexpectedly_empty;
    d.raw_temperature_bit_pos = 0;
    d.raw_temperature = 0;

    ios_set_input (ios_owts_data);
    ios_set (ios_owts_data, 0);

    ios_set_output (ios_owts_debug);
    ios_set (ios_owts_debug, 1);

    owts_isr_stop ();
}

void owts_deinit ()
{
	ios_set_input (ios_owts_data);
}
