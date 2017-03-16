#include <inttypes.h>
#include <board.h>
#include <cr_section_macros.h>
#include <owts.h>
#include <error.h>

#define pin 9
#define debug_pin 7

bool owts_drive_line (enum owts_action action)
{
	Chip_GPIO_SetPinState(LPC_GPIO, 0, debug_pin, 0);

    if (action == owts_wire_pull)
    {
        Chip_GPIO_SetPinState(LPC_GPIO, 0, pin, 0);
        Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, pin);
    }
    else
    {
        Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, pin);
    }
    bool s = Chip_GPIO_GetPinState(LPC_GPIO, 0, pin);

	Chip_GPIO_SetPinState(LPC_GPIO, 0, debug_pin, 1);
    return s;
}

static void (*owts_cb)(uint16_t) = NULL;
void owts_isr_register_call ( void (*cb)(uint16_t), uint16_t timestamp)
{
	if (timestamp > 0)
	{
		Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, timestamp);
		Chip_TIMER_Reset(LPC_TIMER32_0);
		owts_cb = cb;
		return;
	}

	/* TODO Busy loop */
	cb(0);
}

void owts_on_temperature (enum conversion_result result, int32_t temperature)
{
	int32_t integer = temperature / 1000;
	int32_t decimal = temperature - (integer * 1000);

	printf ("On Temperature result: %d / %"PRIi32".%03"PRIi32"\n", result, integer, decimal);
    owts_cb = NULL;
    owts_start_conversion ();
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
		if (owts_cb)
			owts_cb (0);
	}
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
}

int main(void)
{
    Board_Init();
    SystemCoreClockUpdate();

    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, pin, (IOCON_FUNC0 | IOCON_MODE_PULLUP));
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, pin);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, debug_pin);

    Chip_TIMER_Init(LPC_TIMER32_0);
    Chip_TIMER_Disable(LPC_TIMER32_0);
    Chip_TIMER_Reset(LPC_TIMER32_0);
    Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);
    Chip_TIMER_PrescaleSet(LPC_TIMER32_0,  SystemCoreClock / 1000000);
    Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, 0xFFFFFFFF);
    Chip_TIMER_Enable(LPC_TIMER32_0);

    /* Enable timer interrupt */
    NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
    NVIC_EnableIRQ(TIMER_32_0_IRQn);

    owts_init ();
    owts_start_conversion ();

    while(1) {
    }

    return 0 ;
}
