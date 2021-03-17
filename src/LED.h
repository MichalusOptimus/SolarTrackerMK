/*
 * LED.h
 *
 *  Created on: 18.02.2021
 *      Author: Michal
 */


typedef enum {ON = 1, OFF = 0, TOG = 2} LedState;


typedef enum {	LED2 = 0x00000080,		// LED2 on PB7 (built-in Nucleo)
				LED3 = 0x00004000,		// LED3 on PB 14 (built-in Nucleo)
				LED_R = 0x00000001,		// LED_R on PA0 -> red color RGB
				LED_G = 0x00000002,		// LED_G on PA1 -> green color RGB
				LED_B = 0x00000004		// LED_B on PA2 -> blue color RGB
				} LedPin;

//LED2
//#define LED2		0x00000080
//#define LED2_ON		GPIOB->BSRR |= LED2
//#define LED2_OFF	GPIOB->BRR |= LED2
//LED3
//#define LED3		0x00004000
//#define LED3_ON		GPIOB->BSRR |= LED3
//#define LED3_OFF	GPIOB->BRR |= LED3
//LED RGB
//#define LED_R		0x00000001
//#define LED_R_ON	GPIOA->BSRR |= LED_R
//#define LED_R_OFF	GPIOA->BRR |= LED_R
//#define LED_G		0x00000002
//#define LED_G_ON	GPIOA->BSRR |= LED_G
//#define LED_G_OFF	GPIOA->BRR |= LED_G
//#define LED_B		0x00000004
//#define LED_B_ON	GPIOA->BSRR |= LED_B
//#define LED_B_OFF	GPIOA->BRR |= LED_B

// Configure GPIO
void LedInit();

// Write LED state
// Parameters: 	LED pin (LED2, LED3, LED_R, LED_G, LED_B
//				state (ON, OFF, TOG -> toggle)
void LedWrite(LedPin pin, LedState state);




