/*
 * Button.h
 *
 *  Created on: 14.03.2021
 *      Author: Michal
 */

#include "stm32l4xx.h"

typedef enum {	ButtonUp = GPIO_IDR_ID15,		// button up on PE15
				ButtonDown = GPIO_IDR_ID14,		// button down on PE14
				ButtonLeft = GPIO_IDR_ID13,		// button left on PE13
				ButtonRight = GPIO_IDR_ID12		// button right on PE12
				} Button;


void ButtonsInit();

char ButtonUpRead();

char ButtonDownRead();

char ButtonLeftRead();

char ButtonRightRead();
