#include "reacto_factory_config.h"
#include <reacto/context_factory.template>

#include <stdio.h>
#include <inttypes.h>
#include <reacto/context.h>
#include "ios.h"
#include "display.h"
#include "owts.h"
#include "error.h"
#include "events.h"

void application_loop ()
{
    int r = reacto_context_factory(main_loop_strategy_priority_queue,
								   &timed_queue_context,
								   &queues_context);
    if (r < 0)
    {
        printf ("reacto_context_factory failed with %d.", r);
        return;
    }

	display_init ();
	owts_init ();
    events_init ();

    main_loop_run(reacto_context_main_loop());
}
