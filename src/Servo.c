/*
 * Servo.c
 *
 *  Created on: 03.03.2021
 *      Author: Michal
 */

#include "stm32l4xx.h"
#include "Servo.h"

// Initialization servo
void ServoInit()
{
	// Enable clock for TIM3 and GPIO C
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// Configure pin PC8 and PC9 as alternate function
	GPIOC->MODER &= ~GPIO_MODER_MODE8_0; 	// Set PC8 as Alternate function, 0 write in MODER8[1]
	GPIOC->AFR[1] |= GPIO_AFRH_AFSEL8_1; 	// AF 2 on PC8 -> TIM3_CH3

	GPIOC->MODER &= ~GPIO_MODER_MODE9_0; 	// Set PC9 as Alternate function, 0 write in MODER9[1]
	GPIOC->AFR[1] |= GPIO_AFRH_AFSEL9_1; 	// AF 2 on PC9 -> TIM3_CH4


	// Configure TIM3, channel 3 and 4
	TIM3->PSC = 79;				// prescaler 80 MHz / (79 + 1) = 1 MHz
	TIM3->ARR = 20000;			// 20000us = 20ms = 50Hz -> required by servo
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;	// OCxM = 0110 -> PWM mode 1, channel is active when TIM3_CNT < TIM3_CCRx
	TIM3->CCMR2 |= TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;	// output compare preload enable
	TIM3->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;	// capture/compare 3 output enable
	TIM3->CR1 |= TIM_CR1_ARPE;	// auto-reload preload enable
	TIM3->EGR |= TIM_EGR_UG;	// update generation
	TIM3->CR1 |= TIM_CR1_CEN;	// counter enable
}

void ServoSetAngleHorizontal(int angle)
{
	if(angle<0) angle = 0;				// limits
	else if(angle>180) angle = 180;

	TIM3->CCR3 = 11.11 * (180 - angle) + 500; 	// servo can work in range 500 - 2500us, factor 11.11 -> 2000us /180 deg
}

void ServoSetAngleVertical(int angle)
{
	if(angle<0) angle = 0;				// limits
	else if(angle>90) angle = 90;

	TIM3->CCR4 = 11.11 * angle + 500; 	// servo can work in range 500 - 2500ms, factor 11.11 -> 2000ms /180 deg
}
