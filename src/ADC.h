/*
 * ADC.h
 *
 *  Created on: 05.04.2021
 *      Author: Michal
 *
 *	LightSensorUp - PC0 - ADC1/1
 *	LightSensorDown - PC1 - ADC1/2
 *	LightSensorEast - PC4 - ADC1/13
 *	LightSensorWest - PC5 - ADC1/14
 *
 *
 */
//char LightSensorUp = 1;
//char LightSensorDown = 2;
//char LightSensorEast = 13;
//char LightSensorWest = 14;

// Initialization ADC
void ADC_Init();

// Read signal from ADC
// Return value in 12 bits resolution
int readADC(char pinADC);

// Read signal from ADC,
// Return value converted to 3.3 volts
float readADC_Voltage();

