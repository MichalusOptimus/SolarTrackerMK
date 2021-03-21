/*
 * Button.c
 *
 *  Created on: 14.03.2021
 *      Author: Michal
 *
 *      pushbutton UP -> PE15
 *      pushbutton DOWN -> PE14
 *      pushbutton LEFT -> PE12
 *      pushbutton RIGHT -> PE10
 *
 *      pushbuttons connected to GND
 *      enable internal pull-up
 */


#include "Button.h"
#include "SysTickConfig.h"

#define buttonUpPressed (!(GPIOE->IDR & GPIO_IDR_ID14))
#define buttonDownPressed (!(GPIOE->IDR & GPIO_IDR_ID15))
#define buttonLeftPressed (!(GPIOE->IDR & GPIO_IDR_ID12))
#define buttonRightPressed (!(GPIOE->IDR & GPIO_IDR_ID10))

// Button UP
unsigned char stateButtonUp = 1;
int startPressedTimeUp=0, pressedTimeUp = 0, stopPressedTimeUp=0;
// Button DOWN
unsigned char stateButtonDown = 1;
int startPressedTimeDown=0, pressedTimeDown = 0, stopPressedTimeDown=0;
// Button LEFT
unsigned char stateButtonLeft = 1;
int startPressedTimeLeft=0, pressedTimeLeft = 0, stopPressedTimeLeft=0;
// Button RIGHT
unsigned char stateButtonRight = 1;
int startPressedTimeRight=0, pressedTimeRight = 0, stopPressedTimeRight=0;

int debouncingTime = 20;

void ButtonsInit()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN; // enable clock for GPIO E
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable clock for SYSCFG + COMP + VREFBUF

	GPIOE->MODER &= ~(GPIO_MODER_MODE14_0 | GPIO_MODER_MODE14_1); //  configure PE14 [00] -> input mode, button UP
	GPIOE->MODER &= ~(GPIO_MODER_MODE15_0 | GPIO_MODER_MODE15_1); //  configure PE15 [00] -> input mode, button DOWN
	GPIOE->MODER &= ~(GPIO_MODER_MODE10_0 | GPIO_MODER_MODE10_1); //  configure PE10 [00] -> input mode, button LEFT
	GPIOE->MODER &= ~(GPIO_MODER_MODE12_0 | GPIO_MODER_MODE12_1); //  configure PE12 [00] -> input mode, button RIGHT
	GPIOE->MODER &= ~(GPIO_MODER_MODE7_0 | GPIO_MODER_MODE7_1); //    configure PE7 [00] -> input mode, button MODE

	GPIOE->PUPDR |= GPIO_PUPDR_PUPD14_0; // configure PE14 [01] -> internal pull-up
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD15_0; // configure PE15 [01] -> internal pull-up
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD10_0; // configure PE10 [01] -> internal pull-up
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD12_0; // configure PE12 [01] -> internal pull-up
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD7_0; // configure PE7 [01] -> internal pull-up

	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI7_PE; // select PE line for EXTI

	EXTI->FTSR1 |= EXTI_FTSR1_FT7; // falling trigger enable
	EXTI->RTSR1 |= EXTI_RTSR1_RT7; // rising trigger enable

	EXTI->IMR1 |= EXTI_IMR1_IM7; // unmask interrupt line
	EXTI->EMR1 |= EXTI_EMR1_EM7; // event

	NVIC_EnableIRQ(EXTI9_5_IRQn); // enable NVIC interface

}

void EXTI9_5_IRQHandler()
{
	//if(EXTI->PR1 & EXTI_PR1_PIF7)
	//{
		if(GPIOE->IDR & GPIO_IDR_ID7) autoMode = 0;
		else autoMode = 1;

		EXTI->PR1 |= EXTI_PR1_PIF7;
	//}

}

/* Button UP */
char ButtonUpRead()
{
	int currentTime = millis();

	switch(stateButtonUp)
	{
	case 1:
		if(buttonUpPressed)
		{	stateButtonUp = 2;
			startPressedTimeUp = millis();
			pressedTimeUp = 0;
		}
	break;

	case 2:
		if((currentTime - startPressedTimeUp>debouncingTime) && buttonUpPressed) stateButtonUp = 3;
		else if((currentTime - startPressedTimeUp>debouncingTime) && !buttonUpPressed) stateButtonUp = 1;
	break;

	case 3:
		pressedTimeUp = currentTime - startPressedTimeUp;	// debouncing, counting pressed time
		if(!buttonUpPressed) {stateButtonUp = 4; stopPressedTimeUp = millis();}
	break;

	case 4:
		if((currentTime - stopPressedTimeUp>debouncingTime) && buttonUpPressed) stateButtonUp = 3;
		else if((currentTime - stopPressedTimeUp>debouncingTime) && !buttonUpPressed)
		{
		stateButtonUp = 1;
		if((pressedTimeUp >= 50) && (pressedTimeUp <= 1000)) return 1;	// short press, one click
		else if(pressedTimeUp > 1000) return 2;
		}
	break;
	}
}

/* Button DOWN */
char ButtonDownRead()
{
	int currentTime = millis();

	switch(stateButtonDown)
	{
	case 1:
		if(buttonDownPressed)
		{	stateButtonDown = 2;
			startPressedTimeDown = millis();
			pressedTimeDown = 0;
		}
	break;

	case 2:
		if((currentTime - startPressedTimeDown>debouncingTime) && buttonDownPressed) stateButtonDown = 3;
		else if((currentTime - startPressedTimeDown>debouncingTime) && !buttonDownPressed) stateButtonDown = 1;
	break;

	case 3:
		pressedTimeDown = currentTime - startPressedTimeDown;	// debouncing, counting pressed time
		if(!buttonDownPressed) {stateButtonDown = 4; stopPressedTimeDown = millis();}
	break;

	case 4:
		if((currentTime - stopPressedTimeDown>debouncingTime) && buttonDownPressed) stateButtonDown = 3;
		else if((currentTime - stopPressedTimeDown>debouncingTime) && !buttonDownPressed)
		{
		stateButtonDown = 1;
		if((pressedTimeDown >= 50) && (pressedTimeDown <= 1000)) return 1;	// short press, one click
		else if(pressedTimeDown > 1000) return 2;
		}
	break;
	}
}

/* Button LEFT */
char ButtonLeftRead()
{
	int currentTime = millis();

	switch(stateButtonLeft)
	{
	case 1:
		if(buttonLeftPressed)
		{	stateButtonLeft = 2;
			startPressedTimeLeft = millis();
			pressedTimeLeft = 0;
		}
	break;

	case 2:
		if((currentTime - startPressedTimeLeft>debouncingTime) && buttonLeftPressed) stateButtonLeft = 3;
		else if((currentTime - startPressedTimeLeft>debouncingTime) && !buttonLeftPressed) stateButtonLeft = 1;
	break;

	case 3:
		pressedTimeLeft = currentTime - startPressedTimeLeft;	// debouncing, counting pressed time
		if(!buttonLeftPressed) {stateButtonLeft = 4; stopPressedTimeLeft = millis();}
	break;

	case 4:
		if((currentTime - stopPressedTimeLeft>debouncingTime) && buttonLeftPressed) stateButtonLeft = 3;
		else if((currentTime - stopPressedTimeLeft>debouncingTime) && !buttonLeftPressed)
		{
		stateButtonLeft = 1;
		if((pressedTimeLeft >= 50) && (pressedTimeLeft <= 1000)) return 1;	// short press, one click
		else if(pressedTimeLeft > 1000) return 2;
		}
	break;
	}
}

/* Button RIGHT */
char ButtonRightRead()
{
	int currentTime = millis();

	switch(stateButtonRight)
	{
	case 1:
		if(buttonRightPressed)
		{	stateButtonRight = 2;
			startPressedTimeRight = millis();
			pressedTimeRight = 0;
		}
	break;

	case 2:
		if((currentTime - startPressedTimeRight>debouncingTime) && buttonRightPressed) stateButtonRight = 3;
		else if((currentTime - startPressedTimeRight>debouncingTime) && !buttonRightPressed) stateButtonRight = 1;
	break;

	case 3:
		pressedTimeRight = currentTime - startPressedTimeRight;	// debouncing, counting pressed time
		if(!buttonRightPressed) {stateButtonRight = 4; stopPressedTimeRight = millis();}
	break;

	case 4:
		if((currentTime - stopPressedTimeRight>debouncingTime) && buttonRightPressed) stateButtonRight = 3;
		else if((currentTime - stopPressedTimeRight>debouncingTime) && !buttonRightPressed)
		{
		stateButtonRight = 1;
		if((pressedTimeRight >= 50) && (pressedTimeRight <= 1000)) return 1;	// short press, one click
		else if(pressedTimeRight > 1000) return 2;
		}
	break;
	}
}
