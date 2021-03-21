/*
 * Button.h
 *
 *  Created on: 14.03.2021
 *      Author: Michal
 */

#include "stm32l4xx.h"

volatile unsigned char autoMode;

void ButtonsInit();

char ButtonUpRead();

char ButtonDownRead();

char ButtonLeftRead();

char ButtonRightRead();
