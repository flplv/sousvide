#include <reacto/reusables/time.h>
#include <chip.h>

reacto_time_t time_cnt = 0;

reacto_time_t time_now ()
{
	return time_cnt;
}

void SysTick_Handler ()
{
	time_cnt ++;
}

void time_platform_init ()
{
	SysTick_Config(SystemCoreClock / 1000);

}

