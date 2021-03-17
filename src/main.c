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
int previousTimeLED = 0, angleVertical = 0, angleHorizontal = 0;
float longitude = 22, latitude = 49.7;
unsigned char hr = 0,mn = 0,sc = 0,timezone = 1;
int dayOfYear = 200;

float varToSend[]={0,0,0};


int main(void)
{

	/*Init peripherials */
	SysClockHSI_Init();
	SysTickInit();
	USART3_Init();
	LedInit();
	ServoInit();
	ButtonsInit();

  /* Infinite loop */
  while (1)
  {
	  if(ButtonUpRead() == 1) {LedWrite(LED_B,TOG); angleVertical +=5;}
	  if(ButtonDownRead() == 1) {LedWrite(LED_R,TOG); angleVertical -=5;}
	  if(ButtonLeftRead() == 1) {LedWrite(LED2,TOG); angleHorizontal +=5;}
	  if(ButtonRightRead() == 1) {LedWrite(LED3,TOG); angleHorizontal -=5;}

	  ServoSetAngleHorizontal(angleHorizontal);
	  ServoSetAngleVertical(angleVertical);

	  // TESTING
	 /*if(timeElapsed(500,previousTimeLED))
	 {
		 SunCalculatePosition(timezone,dayOfYear,hr,mn,sc,longitude,latitude);
		 LedWrite(LED2,TOG);
	  	 previousTimeLED = millis();

	  	//ServoSetAngleHorizontal(SunGetCurrentAzimuth()- 90.0);
	  	//ServoSetAngleVertical(SunGetCurrentZenith());
	  	varToSend[0]=SunGetCurrentAzimuth();
	  	varToSend[1]=SunGetCurrentZenith();
	  	varToSend[2]=(float)hr;
	  	USART3_SendData(varToSend,bufSend,(char)sizeof(bufSend));
	  	//USART3_SendChar('\n');
	  	hr +=1;
	  	if(hr==23) hr = 0;
	 }*/

  }
}
