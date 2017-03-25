#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <platform.h>
#include <ios.h>
#include <priorities.h>
#include <owts.h>
#include <display.h>

void display_isr_start ()
{
	Chip_TIMER_Enable (LPC_TIMER16_0);
}

void display_isr_stop ()
{
	Chip_TIMER_Disable (LPC_TIMER16_0);
}

void TIMER16_0_IRQHandler ()
{
	if (Chip_TIMER_MatchPending(LPC_TIMER16_0, 0))
	{
		Chip_TIMER_ClearMatch(LPC_TIMER16_0, 0);

		display_ios_periodic ();
	}

	NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
}

void display_platform_init ()
{
	Chip_TIMER_Init(LPC_TIMER16_0);
	Chip_TIMER_Disable(LPC_TIMER16_0);
	Chip_TIMER_Reset(LPC_TIMER16_0);
	Chip_TIMER_MatchEnableInt(LPC_TIMER16_0, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 0);
	Chip_TIMER_PrescaleSet(LPC_TIMER16_0,  SystemCoreClock / 1000);
	Chip_TIMER_SetMatch(LPC_TIMER16_0, 0, 1);  /* ms */

	/* Enable timer interrupt */
	NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);
	NVIC_SetPriority(TIMER_16_0_IRQn, priorities_display);
	NVIC_EnableIRQ(TIMER_16_0_IRQn);
}
