/*
 * 	SysTick Configuration
 * 	Tick every 1 ms
 * 	80MHz / 1000 -> 80000 ticks/ms
 */


#define F_CLK 80000000					// Clock frequency
#define SYSTICK_CONFIG_MS F_CLK/1000	// = 80 000 -> 1 ms

// Configuration SysTick registers
void SysTickInit();

// Return current time in milliseconds
uint32_t millis();

// Return 1 if time ms elapsed from the previousTime
char timeElapsed(uint32_t ms, uint32_t previousTime);
