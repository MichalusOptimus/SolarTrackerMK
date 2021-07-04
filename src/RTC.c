/*
 * RTC.c
 *
 *  Created on: 04.05.2021
 *      Author: Michal
 */

#include "stm32l4xx.h"
#include "RTC.h"


void RTC_Init()
{
	// power interface clock enable
	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;

	// access to RTC and Backup registers enabled
	PWR->CR1 |= PWR_CR1_DBP;

	// enable LSI oscillator and wait for confirmation
	//RCC->CSR |= RCC_CSR_LSION;
	//while(!(RCC->CSR & RCC_CSR_LSIRDY));
	// LSI oscillator clock used as RTC clock
	// RCC->BDCR = 0;
	//RCC->BDCR |= RCC_BDCR_RTCSEL_1;
	//RCC->BDCR |= RCC_BDCR_RTCEN;

	// enable LSE oscillator and wait for confirmation
	RCC->BDCR |= RCC_BDCR_LSEON;
	while(!(RCC->BDCR & RCC_BDCR_LSERDY));
	// LSE oscillator clock used as RTC clock
	RCC->BDCR |= (1 << RCC_BDCR_RTCSEL_Pos);
	RCC->BDCR |= RCC_BDCR_RTCEN;
}

void RTC_SetTime(unsigned char hr, unsigned char mn, unsigned char sc)
{
	// unlock the write protection
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// enter to initialization mode and wait for confirmation
	RTC->ISR |= RTC_ISR_INIT;
	while(!(RTC->ISR & RTC_ISR_INITF));

	RTC->PRER = 0;
	RTC->PRER = (127 << 16) | (255 << 0); 	// synchronus and asynchronus prescalers

	RTC->CR |= RTC_CR_BYPSHAD;

	RTC->TR = 0; 			// Clear register
	RTC->TR |= (hr/10) << 20; 	// Hour tens
	RTC->TR |= (hr%10) << 16; 	// Hour units
	RTC->TR |= (mn/10) << 12; 	// Minute tens
	RTC->TR |= (mn%10) << 8; 	// Minute units
	RTC->TR |= (sc/10) << 4; 	// Second tens
	RTC->TR |= (sc%10) << 0; 	// Second units

	// 24 hour/day format
	RTC->CR &= ~RTC_CR_FMT;

	RTC->CR &= ~RTC_CR_BYPSHAD;

	// exit initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// wait for synchronization
	RTC->ISR &= ~RTC_ISR_RSF;
	while (!(RTC->ISR & RTC_ISR_RSF));

	// enable write protection
	RTC->WPR = 0xFF;
}

// Set date
void RTC_SetDate(unsigned char day, unsigned char month, unsigned char year)
{
	// unlock the write protection
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// enter to initialization mode and wait for confirmation
	RTC->ISR |= RTC_ISR_INIT;
	while(!(RTC->ISR & RTC_ISR_INITF));

	RTC->PRER = 0;
	RTC->PRER = (127 << 16) | (255 << 0); 	// synchronus and asynchronus prescalers

	RTC->CR |= RTC_CR_BYPSHAD;

	RTC->DR = 0; 					// Clear register
	RTC->DR |= (year/10) << 20; 	// Year tens
	RTC->DR |= (year%10) << 16; 	// Year units
	RTC->DR |= (month/10) << 12; 	// Month tens
	RTC->DR |= (month%10) << 8; 	// Month units
	RTC->DR |= (day/10) << 4; 		// Day tens
	RTC->DR |= (day%10) << 0; 		// Day units

	RTC->CR &= ~RTC_CR_BYPSHAD;

	// exit initialization mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// wait for synchronization
	RTC->ISR &= ~RTC_ISR_RSF;
	while (!(RTC->ISR & RTC_ISR_RSF));

	// enable write protection
	RTC->WPR = 0xFF;
}

// Reading time from RTC
void RTC_ReadTime(unsigned char *hr, unsigned char *mn, unsigned char *sc)
{
	*sc = 10 *((RTC->TR & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos) + (RTC->TR & RTC_TR_SU_Msk); //Seconds
	*mn = 10 *((RTC->TR & RTC_TR_MNT_Msk) >> RTC_TR_MNT_Pos) + ((RTC->TR & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos); // Minutes
	*hr = 10 *((RTC->TR & RTC_TR_HT_Msk) >> RTC_TR_HT_Pos) + ((RTC->TR & RTC_TR_HU_Msk) >> RTC_TR_HU_Pos); // Hours
}

// Reading date from RTC
void RTC_ReadDate(unsigned char *day, unsigned char *month, unsigned char *year)
{
	*day = 10 *((RTC->DR & RTC_DR_DT_Msk) >> RTC_DR_DT_Pos) + (RTC->DR & RTC_DR_DU_Msk); // Day
	*month = 10 *((RTC->DR & RTC_DR_MT_Msk) >> RTC_DR_MT_Pos) + ((RTC->DR & RTC_DR_MU_Msk) >> RTC_DR_MU_Pos); // Month
	*year = 10 *((RTC->DR & RTC_DR_YT_Msk) >> RTC_DR_YT_Pos) + ((RTC->DR & RTC_DR_YU_Msk) >> RTC_DR_YU_Pos); // Year
}

short dayYear(char day, char month)
{
	if(month == 1) return day;											// January
	else if(month == 2) return (31+day);								// February
	else if(month == 3) return (31+28+day);								// March
	else if(month == 4) return (31+28+31+day);							// April
	else if(month == 5) return (31+28+31+30+day);						// May
	else if(month == 6) return (31+28+31+30+31+day);					// June
	else if(month == 7) return (31+28+31+30+31+30+day);					// July
	else if(month == 8) return (31+28+31+30+31+30+31+day);				// August
	else if(month == 9) return (31+28+31+30+31+30+31+31+day);			// September
	else if(month == 10) return (31+28+31+30+31+30+31+31+30+day);		// October
	else if(month == 11) return (31+28+31+30+31+30+31+31+30+31+day);	// November
	else if(month == 12) return (31+28+31+30+31+30+31+31+30+31+30+day);	// December
}
