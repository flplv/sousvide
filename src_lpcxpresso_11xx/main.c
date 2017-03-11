#include <board.h>
#include <cr_section_macros.h>
#include <owts.h>
#include <error.h>

bool owts_drive_line (enum owts_action action)
{
    if (action == owts_wire_pull)
    {
        Chip_GPIO_SetPinState(LPC_GPIO, 0, 7, 0);
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 7);
    }
    else
    {
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 7);
    }

    return Chip_GPIO_GetPinState(LPC_GPIO, 0, 7);
}

static void (*owts_cb)(uint16_t) = NULL;
void owts_isr_register_call ( void (*cb)(uint16_t), uint16_t timestamp)
{
	if (timestamp == 0)
		timestamp = 1;

    Chip_TIMER_Reset(LPC_TIMER32_0);
    Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, (Chip_Clock_GetSystemClockRate() / 1000000 * timestamp));
    owts_cb = cb;
    Chip_TIMER_Enable(LPC_TIMER32_0);
}

void owts_on_temperature (enum conversion_result result, uint16_t temperature)
{
	printf ("On Temperature result: %d / %d\n", result, temperature);
    owts_cb = NULL;
}

void error (enum errors err)
{
    printf ("Error %d\n", err);
}

void TIMER32_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1))
	{
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);
	    Chip_TIMER_Disable (LPC_TIMER32_0);

		if (owts_cb) owts_cb (0);
	}

	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
}

int main(void)
{
    SystemCoreClockUpdate();
    Board_Init();

    Chip_IOCON_PinMuxSet(LPC_GPIO, 0, 7, (IOCON_FUNC0 | IOCON_MODE_PULLUP));

    Chip_TIMER_Init(LPC_TIMER32_0);
    Chip_TIMER_Disable(LPC_TIMER32_0);
    Chip_TIMER_Reset(LPC_TIMER32_0);
    Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 1);

    /* Enable timer interrupt */
    NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
    NVIC_EnableIRQ(TIMER_32_0_IRQn);

    owts_init ();
    owts_start_conversion ();

    while(1) {
    }

    return 0 ;
}
