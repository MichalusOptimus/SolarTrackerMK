/*
 * ADC.c
 *
 *  Created on: 21.02.2021
 *      Author: Michal
 */

#include <ADC.h>
#include "stm32l4xx.h"

/* ADC */
void ADC_Init()
{

RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_ADCEN; 	// enable ADC and GPIO C clock
RCC->CCIPR |= RCC_CCIPR_ADCSEL_0 | RCC_CCIPR_ADCSEL_1;		// select clock ADC

ADC1->CFGR |= ADC_CFGR_DISCEN;			// discontinuous conversion mode

//ADC1->IER |= ADC_IER_EOSIE;			// enable End Of Conversion interrupt
//NVIC_EnableIRQ(ADC1_IRQn);

//ADC1->SQR1 |= (4<< ADC_SQR1_SQ1_Pos);	// 4 channel
//ADC1->SMPR1 |= ADC_SMPR1_SMP4_1;		// 010 -> 12.5 ADC clock cycles, RESET STATE bits(000) -> 2.5 ADC clock cycles

ADC1->CR = ADC_CR_ADEN | ADC_CR_ADVREGEN;	// enable ADC and voltage regulator
while(!(ADC1->ISR & ADC_ISR_ADRDY));		// wait for ADC ready
}


int readADC(char pinADC)
{
	ADC1->SQR1 = 0; // reset value
	ADC1->SQR1 |= (pinADC << ADC_SQR1_SQ1_Pos);	// select channel
	ADC1->CR |= ADC_CR_ADSTART;			// start measurement
	while(!(ADC1->ISR & ADC_ISR_EOS));	// wait for end of sequence
	int ADC_Val = ADC1->DR;				// read DR register
	ADC1->ISR |= ADC_ISR_EOS;			// clear flag
	return ADC_Val;
}


float readADC_Voltage()
{
	ADC1->CR |= ADC_CR_ADSTART;				// start measurement
	while(!(ADC1->ISR & ADC_ISR_EOS));		// wait for end of sequence
	int ADC_Val = ADC1->DR;					// read DR register
	ADC1->ISR |= ADC_ISR_EOS;				// clear flag

	float VCC = 3.3;						// convert read ADC value to 3.3 volts, 12 bits resulotion = 4095
	float temp = (((float)ADC_Val)/4095.0);
	temp = temp * VCC;

	return temp;
}

