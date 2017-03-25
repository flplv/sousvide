#include <platform.h>
#include <priorities.h>
#include <owts.h>

static void (*owts_cb)(uint16_t) = NULL;

void owts_isr_next_call (uint16_t timestamp)
{
	if (timestamp > 0 || !owts_cb)
	{
		Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, timestamp);
		Chip_TIMER_Reset(LPC_TIMER32_0);
		return;
	}

	/* TODO Busy loop */
	owts_cb (0);
}

void owts_isr_stop ()
{
	owts_cb = NULL;
	Chip_TIMER_Disable (LPC_TIMER32_0);
}

void owts_isr_start (void (*cb)(uint16_t))
{
	owts_cb = cb;
	Chip_TIMER_Enable (LPC_TIMER32_0);
}

void TIMER32_0_IRQHandler(void)
{
	Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);
	if (owts_cb)
		owts_cb (0);

	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
}

void owts_platform_init ()
{
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_Disable(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);
	Chip_TIMER_PrescaleSet(LPC_TIMER32_0,  SystemCoreClock / 1000000);
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, 0xFFFFFFFF);

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
	NVIC_SetPriority(TIMER_32_0_IRQn, priorities_owts);
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}
