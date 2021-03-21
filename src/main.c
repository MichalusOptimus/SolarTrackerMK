/* Includes */
#include "stm32l4xx.h"
#include "RccConfig.h"
#include "LED.h"
#include "SysTickConfig.h"
#include "Servo.h"
#include "USART3.h"
#include "Button.h"
#include "SunPosition.h"


/* Variables */
int previousTime = 0;
float angleVertical = 0, angleHorizontal = 0;
float longitude = 22, latitude = 49.7;
unsigned char hr = 0,mn = 0,sc = 0,timezone = 1;
int dayOfYear = 200;
//char autoMode = 0;

float varToSend[]={0,0,0};


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

	/* Check witch */
	if(GPIOE->IDR & GPIO_IDR_ID7) autoMode = 0;	// check switch position
	else autoMode = 1;

  /* Infinite loop */
  while (1)
  {
	// TESTING
	if(autoMode)
	{

	if(timeElapsed(500,previousTime))
	 {
		SunCalculatePosition(timezone,dayOfYear,hr,mn,sc,longitude,latitude);
		LedWrite(LED_G,TOG);
		LedWrite(LED_R,OFF);

	  	angleHorizontal = SunGetCurrentAzimuth();
	  	angleVertical = SunGetCurrentZenith();
	  	angleHorizontal = angleHorizontal- 90.0;

	  	ServoSetAngleHorizontal(angleHorizontal);
	  	ServoSetAngleVertical(angleVertical);
	  	varToSend[0]=SunGetCurrentAzimuth();
	  	varToSend[1]=SunGetCurrentZenith();
	  	varToSend[2]=(float)hr;
	  	USART3_SendData(varToSend,bufSend,(char)sizeof(bufSend));
	  	//USART3_SendChar('\n');
	  	hr +=1;
	  	if(hr==23) hr = 0;
	  	previousTime = millis();
	 }
	 }
	 else
	 {
		 LedWrite(LED_R,ON);
		 LedWrite(LED_G,OFF);
		 if(ButtonUpRead() == 1) {LedWrite(LED2,TOG); angleVertical +=5;}
		 if(ButtonDownRead() == 1) {LedWrite(LED2,TOG); angleVertical -=5;}
		 if(ButtonLeftRead() == 1) {LedWrite(LED3,TOG); angleHorizontal +=5;}
		 if(ButtonRightRead() == 1) {LedWrite(LED3,TOG); angleHorizontal -=5;}

		 // Limits
		 if(angleVertical > 90) angleVertical = 90;
		 else if(angleVertical < 0) angleVertical = 0;
		 if(angleHorizontal > 180) angleHorizontal = 180;
		 else if(angleHorizontal < 0) angleHorizontal = 0;


		 ServoSetAngleHorizontal(angleHorizontal);
		 ServoSetAngleVertical(angleVertical);
	 }

  }
}
