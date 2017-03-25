#include <application.h>
#include <platform.h>
#include <ios.h>

int main(void)
{
    SystemCoreClockUpdate();

    time_platform_init ();
    ios_platform_init ();
    owts_platform_init ();
    display_platform_init ();

    application_loop ();
}
