/*
 * 	SysTick Configuration
 *
 */

#include "stm32l4xx.h"
#include "SysTickConfig.h"

volatile int ticks;

// Configuration SysTick registers
void SysTickInit()
{
	SysTick->LOAD = SYSTICK_CONFIG_MS - 1;			// Number of cycles between interrupts

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;		// Enable interrupt
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;	// Enable clock system AHB
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;		// Enable SysTick
}

void SysTick_Handler()
{
	ticks++;
}

// Return current time in milliseconds
uint32_t millis()
{
	return ticks;
}

// Return 1 if time ms elapsed from the previousTime
char timeElapsed(uint32_t ms, uint32_t previousTime)
{
	uint32_t time = millis();
	if(time - previousTime >= ms) return 1;
	else return 0;
}
