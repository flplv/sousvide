#include "reacto_factory_config.h"
#include <reacto/context.h>
#include "ios.h"
#include "display.h"
#include "owts.h"
#include "encoder.h"
#include "ui.h"
#include "error.h"
#include "events.h"
#include "control.h"

void application_loop ()
{
    display_init ();
    owts_init ();
    events_init ();
    ui_init ();
    encoder_init ();
    control_init ();

    main_loop_run(reacto_context_main_loop());
}
