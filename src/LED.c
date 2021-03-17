/*
 * LED.c
 *
 *  Created on: 18.02.2021
 *      Author: Michal
 */

#include "stm32l4xx.h"
#include "LED.h"


void LedInit()
{
	// 1. Enable clock on port B and A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOAEN;

	// 2. Set outputs
	// Default GPIO->MODER = 0xFFFF,
	GPIOB->MODER &= ~GPIO_MODER_MODE7_1; 	// Set PB7 as output, bits(15:14)=(0:1)
	GPIOB->MODER &= ~GPIO_MODER_MODE14_1; 	// Set PB14 as output, bits(29:28)=(0:1)
	GPIOA->MODER &= ~GPIO_MODER_MODE0_1;	// Set PA0 as output, bits(1:0)=(0:1)
	GPIOA->MODER &= ~GPIO_MODER_MODE1_1;	// Set PA1 as output, bits(3:2)=(0:1)
	GPIOA->MODER &= ~GPIO_MODER_MODE2_1;	// Set PA2 as output, bits(5:4)=(0:1)
}

// Write LED state
void LedWrite(LedPin pin, LedState state)
{
	switch(pin)
	{
	case LED2:
		if(state == ON) GPIOB->BSRR |= LED2;
		else if(state == OFF) GPIOB->BRR |= LED2;
		else if(state == TOG) GPIOB->ODR ^= LED2;
	break;
	case LED3:
		if(state == ON) GPIOB->BSRR |= LED3;
		else if(state == OFF) GPIOB->BRR |= LED3;
		else if(state == TOG) GPIOB->ODR ^= LED3;
	break;
	case LED_R:
		if(state == ON) GPIOA->BSRR |= LED_R;
		else if(state == OFF) GPIOA->BRR |= LED_R;
		else if(state == TOG) GPIOA->ODR ^= LED_R;
	break;
	case LED_G:
		if(state == ON) GPIOA->BSRR |= LED_G;
		else if(state == OFF) GPIOA->BRR |= LED_G;
		else if(state == TOG) GPIOA->ODR ^= LED_G;
	break;
	case LED_B:
		if(state == ON) GPIOA->BSRR |= LED_B;
		else if(state == OFF) GPIOA->BRR |= LED_B;
		else if(state == TOG) GPIOA->ODR ^= LED_B;
	break;
	}
}


