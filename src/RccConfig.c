/*
 * System Clock Configuration
 * 		System Clock source = PLL (HSI)
 * 		HSI (Hz)			= 16000000
 * 		SYSCLK (Hz)			= 80000000
 * 		HCLK (Hz)			= 80000000
 * 		AHB Prescaler		= 1
 * 		APB1 Prescaler		= 1
 * 		APB2 Prescaler		= 1
 * 		PLL_M				= 2
 * 		PLL_N				= 20
 * 		PLL_R				= 2
 *
 */

#include "stm32l4xx.h"
#include "RccConfig.h"


void SysClockHSI_Init()
{
	// 1. Enable HSI 16MHz and wait for confirmation
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY));

	// 2. Power interface clock enable
	//RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;

	// 3. Configure the FLASH and the LATENCY Related Settings
	//FLASH->ACR |= FLASH_ACR_ICEN; // instruction cache enable, RESET VALUE
	//FLASH->ACR |= FLASH_ACR_DCEN; // data cache enable, RESET VALUE
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS; // 4 wait states (number of HCLK period to the Flash access time)

	// 4. Configure PRESCALERS HCLK, PCLK1, PCLK2
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // AHB prescaler, HCLK
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1; // APB1 prescaler, PCLK1
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 prescaler, PCLK2

	// 5. Configure PLL
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI; 					// set source PLL as HSI
	RCC->PLLCFGR |=	RCC_PLLCFGR_PLLM_0; 						// PLLM(6:4)=(001) -> PLLM = 2
	RCC->PLLCFGR |=	RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_4;	// PLLN(14:8)=(0010100) -> PLLN = 20
	//RCC->PLLCFGR |=	RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_3;
																// PLLR(26:25)=(00) -> PLLR = 2 RESET VALUE
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;							// PLLCLK output ENABLE

	// 6. Enable PLL and wait for confirmation
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// 7. Select the Clock Source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL );
}
