/* Includes */
#include "stm32l4xx.h"
#include "RccConfig.h"
#include "LED.h"
#include "SysTickConfig.h"
#include "Servo.h"
#include "USART3.h"
#include "Button.h"
#include "ADC.h"
#include "SunPosition.h"


/* Variables */
int previousTime = 0;
float angleVertical = 0, angleHorizontal = 0;
float longitude = 22, latitude = 49.7;
// Time
unsigned char hr = 0,mn = 0,sc = 0,timezone = 1;
int dayOfYear = 200;
// Light sensors
int lightSensorUpIntensity = 0, lightSensorDownIntensity = 0, lightSensorEastIntensity = 0, lightSensorWestIntensity = 0;
const char lightSensorUpChannel = 1, lightSensorDownChannel = 2, lightSensorEastChannel = 13, lightSensorWestChannel = 14;
int rangeLightIntensity = 100;

float varToSend[]={0,0,0};

/* Functions */
 void AlgorithmLightSensors();
 void AlgorithmSunCalculatePosition();


int main(void)
{
	/*Init flags */

	/*Init peripherials */
	SysClockHSI_Init();
	SysTickInit();
	USART3_Init();
	LedInit();
	ServoInit();
	ButtonsInit();
	ADC_Init();

	/* Check switch */
	if(GPIOE->IDR & GPIO_IDR_ID7) autoMode = 0;	// check switch position
	else autoMode = 1;

  /* Infinite loop */
  while (1)
  {
	// TESTING
	if(autoMode)
	{

	if(timeElapsed(200,previousTime))
	 {

		LedWrite(LED_B,TOG);
		LedWrite(LED_R,OFF);

		// Algorithm light sensors
		//AlgorithmLightSensors();

		// Algorithm Sun calculate position
		AlgorithmSunCalculatePosition();

		// Set servo position
	  	ServoSetAngleHorizontal(angleHorizontal);
	  	ServoSetAngleVertical(angleVertical);

	  	// Send message USART
	  	//varToSend[0]=SunGetCurrentAzimuth();
	  	//varToSend[1]=SunGetCurrentZenith();
	  	//varToSend[2]=(float)hr;
	  	//USART3_SendData(varToSend,bufSend,(char)sizeof(bufSend));
	  	//USART3_SendChar('\n');

	  	// simulation of time
	  	hr +=1;
	  	if(hr==23) hr = 0;

	  	previousTime = millis();
	 }
	 }
	// manual mode
	 else
	 {
		 LedWrite(LED_R,ON);
		 LedWrite(LED_B,OFF);
		 LedWrite(LED_G,OFF);

		 // check pushbuttons
		 if(ButtonUpRead() == 1) {LedWrite(LED2,TOG); angleVertical +=5;}
		 if(ButtonDownRead() == 1) {LedWrite(LED2,TOG); angleVertical -=5;}
		 if(ButtonLeftRead() == 1) {LedWrite(LED3,TOG); angleHorizontal +=5;}
		 if(ButtonRightRead() == 1) {LedWrite(LED3,TOG); angleHorizontal -=5;}

		 // Limits
		 if(angleVertical > 90) angleVertical = 90;
		 else if(angleVertical < 0) angleVertical = 0;
		 if(angleHorizontal > 180) angleHorizontal = 180;
		 else if(angleHorizontal < 0) angleHorizontal = 0;

		 // set servo position
		 ServoSetAngleHorizontal(angleHorizontal);
		 ServoSetAngleVertical(angleVertical);

	 }

  }
}

void AlgorithmLightSensors()
{
	// Reading light sensors
	lightSensorUpIntensity = readADC(lightSensorUpChannel) + 150;	// offset -> 150
	lightSensorDownIntensity = readADC(lightSensorDownChannel);
	lightSensorWestIntensity = readADC(lightSensorWestChannel);
	lightSensorEastIntensity = readADC(lightSensorEastChannel);

	// Regulation of horizontal axis
	if((lightSensorEastIntensity - lightSensorWestIntensity < 100) && (lightSensorEastIntensity - lightSensorWestIntensity > -100)) LedWrite(LED_G,ON);
	else if(lightSensorEastIntensity > lightSensorWestIntensity) {angleHorizontal -= 2; LedWrite(LED_G,TOG);}
	else {angleHorizontal += 2; LedWrite(LED_G,TOG);}

	// Regulation of vertical axis
	if((lightSensorUpIntensity - lightSensorDownIntensity < 100) && (lightSensorUpIntensity - lightSensorDownIntensity > -100)) LedWrite(LED_G,ON);
	else if(lightSensorUpIntensity > lightSensorDownIntensity) {angleVertical -= 2; LedWrite(LED_G,TOG);}
	else {angleVertical += 2; LedWrite(LED_G,TOG);}

	// Limits
	if(angleVertical > 90) angleVertical = 90;
	else if(angleVertical < 0) angleVertical = 0;
	if(angleHorizontal > 180) angleHorizontal = 180;
	else if(angleHorizontal < 0) angleHorizontal = 0;
}

void AlgorithmSunCalculatePosition()
{
	// Calculate Sun position
	SunCalculatePosition(timezone,dayOfYear,hr,mn,sc,longitude,latitude);

	// Assign value
	angleHorizontal = SunGetCurrentAzimuth() - 90.0;	// -90 -> function return angle from N, device has max range from E to W
	angleVertical = SunGetCurrentZenith();

}
