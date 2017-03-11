#include <board.h>
#include <cr_section_macros.h>
#include <owts.h>
#include <error.h>

int main(void)
{
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, true);

    owts_init ();

    volatile static int i = 0 ;
    while(1) {
        i++ ;
    }

    return 0 ;
}

bool owts_drive_line (enum owts_action action)
{
    return 0;
}

void owts_isr_register_call ( void (*cb)(uint16_t), uint16_t timestamp)
{

}

void owts_on_temperature (enum conversion_result result, uint16_t timestamp)
{

}

void error (enum errors err)
{
    printf ("Error %d\n", err);
}
