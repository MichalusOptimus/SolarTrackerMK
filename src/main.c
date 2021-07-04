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
#include "RTC.h"

#define LATITUDE  50.02691				// latitude of the current location
#define LONGITUDE 21.98531				// longitude of the current location
#define TIME_BETWEEN_SEND_USART 300		// time between sending message USART in miliseconds


/* Variables */
int previousTimeAutoMode = 0, previousTimeSendingUSART = 0, previousTimeCounting = 0, previousTimeReadSensor = 0;
float angleVertical = 0, angleHorizontal = 0;
// Time
unsigned char hr = 0,mn = 0,sc = 0, day = 0, month = 0, year = 0;
char timezone = 2;
// Light sensors
int lightSensorUpIntensity = 0, lightSensorDownIntensity = 0, lightSensorEastIntensity = 0, lightSensorWestIntensity = 0;
const char lightSensorUpChannel = 1, lightSensorDownChannel = 2, lightSensorEastChannel = 13, lightSensorWestChannel = 14;
int rangeLightIntensity = 50;

// Send msg UART
float bufToSend[NUM_OF_FLOATS_SEND_USART];

/* Functions */
void ClearBufToSend();
void ReadLightSensors();
void ReadPushbuttons();
void AlgorithmLightSensors();
void AlgorithmSunCalculatePosition();
unsigned char charToNum(unsigned char sign);


int main(void)
{
	/*Init flags */
	receivedMessageFromUSART = 0;

	/*Init peripherials */
	SysClockHSI_Init();
	RTC_Init();
	RTC_SetTime(14,20,0);
	RTC_SetDate(4,5,23);
	SysTickInit();
	USART3_Init();
	LedInit();
	ServoInit();
	ButtonsInit();
	ADC_Init();

	/* Check switch autoMode */
	if(GPIOE->IDR & GPIO_IDR_ID7) autoMode = 0;	// check switch position
	else autoMode = 1;

  /* Infinite loop */
  while (1)
  {
	// 'TASK 1' - Auto mode
	if(autoMode)
	{
	if(timeElapsed(15,previousTimeAutoMode))
	 {
		LedWrite(LED_AUTO,ON);
		LedWrite(LED_MAN,OFF);

		// Algorithm light sensors
		//AlgorithmLightSensors();

		// Algorithm Sun calculate position
		AlgorithmSunCalculatePosition();

		// Set servo position
	  	ServoSetAngleHorizontal(angleHorizontal);
	  	ServoSetAngleVertical(angleVertical);

	  	previousTimeAutoMode = millis();
	 }
	 }
	// manual mode
	 else
	 {
		 LedWrite(LED_MAN,ON);
		 LedWrite(LED_AUTO,OFF);

		 ReadPushbuttons();

		 // Limits
		 if(angleVertical > 90) angleVertical = 90;
		 else if(angleVertical < 0) angleVertical = 0;
		 if(angleHorizontal > 180) angleHorizontal = 180;
		 else if(angleHorizontal < 0) angleHorizontal = 0;

		 // set servo position
		 ServoSetAngleHorizontal(angleHorizontal);
		 ServoSetAngleVertical(angleVertical);
	 }

	// 'TASK 2' - Read light sensor
	if(timeElapsed(10,previousTimeReadSensor))
	{
		ReadLightSensors();
		previousTimeReadSensor = millis();
	}

	// 'TASK 3' - Send message USART
	if(timeElapsed(TIME_BETWEEN_SEND_USART,previousTimeSendingUSART))
	{
	// Reading and calculate only to sending
	RTC_ReadTime(&hr, &mn, &sc);
	RTC_ReadDate(&day,&month, &year);
	SunCalculatePosition(timezone,dayYear(day,month),hr,mn,sc,LONGITUDE,LATITUDE);

	// Write data to buf
	ClearBufToSend();
	bufToSend[0]= (float)hr;					// current hour
	bufToSend[1]= (float)mn;					// current minute
	bufToSend[2]= (float)sc;					// current second
	bufToSend[3]= (float)day;					// current day
	bufToSend[4]= (float)month;					// current month
	bufToSend[5]= (float)year;					// current year
	bufToSend[6]= SunGetCurrentAzimuth();		// calculate position
	bufToSend[7]= 90.0- SunGetCurrentZenith();	// calculate position
	bufToSend[8]= 90.0 + angleHorizontal;		// current position horizontal (angle from N clockwise -> azimuth)
	bufToSend[9]= 90.0 - angleVertical;			// current position vertical (angle from vertical)
	bufToSend[10]=(float)lightSensorUpIntensity;
	bufToSend[11]=(float)lightSensorDownIntensity;
	bufToSend[12]=(float)lightSensorWestIntensity;
	bufToSend[13]=(float)lightSensorEastIntensity;

	USART3_SendData(bufToSend,bufSend,(char)sizeof(bufSend));

	previousTimeSendingUSART = millis();
	}

	// 'TASK 4' - Analyze received frame
	if(receivedMessageFromUSART)
	{
		unsigned char calcHr, calcMn; // temp var time
		unsigned char calcDay,calcMonth, calcYear;	// temp var date

		switch(bufReceive[0])
		{
		// Set time
		case 'T': 	calcHr = charToNum(bufReceive[1])*10 + charToNum(bufReceive[2]);
					calcMn = charToNum(bufReceive[3])*10 + charToNum(bufReceive[4]);
					if(calcHr<0 || calcHr >23 || calcMn<0 || calcMn>59) USART3_SendChar('N');
					else
					{
						USART3_SendChar('Y');
						RTC_SetTime(calcHr,calcMn,0);
					}
		break;

		// Set date
		case 'D':	calcDay = charToNum(bufReceive[1])*10 + charToNum(bufReceive[2]);
					calcMonth = charToNum(bufReceive[3])*10 + charToNum(bufReceive[4]);
					calcYear = charToNum(bufReceive[5])*10 + charToNum(bufReceive[6]);
					if(calcDay<0 || calcDay >31 || calcMonth<0 || calcMonth>12 || calcYear<0 || calcYear >99) USART3_SendChar('N');
					else
					{
						USART3_SendChar('Y');
						RTC_SetDate(calcDay,calcMonth,calcYear);
					}
		break;
		}


		LedWrite(LED3,TOG);
		receivedMessageFromUSART = 0;
	}
  }
}
////// END MAIN ///////

// Clear buffer to send USART -> set 0
void ClearBufToSend()
{
	int i;
	for (i=0;i<NUM_OF_FLOATS_SEND_USART;i++)
	{
		bufToSend[i] = 0;
	}
}
// Reading from light sensors
void ReadLightSensors()
{
	lightSensorUpIntensity = readADC(lightSensorUpChannel);
	lightSensorDownIntensity = readADC(lightSensorDownChannel);
	lightSensorWestIntensity = readADC(lightSensorWestChannel);
	lightSensorEastIntensity = readADC(lightSensorEastChannel);
}

// Reading from pushbuttons
void ReadPushbuttons()
{
if(ButtonUpRead() == 1) {LedWrite(LED2,TOG); angleVertical +=5;}
if(ButtonDownRead() == 1) {LedWrite(LED2,TOG); angleVertical -=5;}
if(ButtonLeftRead() == 1) {LedWrite(LED3,TOG); angleHorizontal +=5;}
if(ButtonRightRead() == 1) {LedWrite(LED3,TOG); angleHorizontal -=5;}
}

void AlgorithmLightSensors()
{

	// Regulation of horizontal axis
	if((lightSensorEastIntensity - lightSensorWestIntensity < rangeLightIntensity) && (lightSensorEastIntensity - lightSensorWestIntensity > -rangeLightIntensity)) LedWrite(LED_G,ON);
	else if(lightSensorEastIntensity > lightSensorWestIntensity) {angleHorizontal -= 1; LedWrite(LED_G,TOG);}
	else {angleHorizontal += 1; LedWrite(LED_G,TOG);}

	// Regulation of vertical axis
	if((lightSensorUpIntensity - lightSensorDownIntensity < rangeLightIntensity) && (lightSensorUpIntensity - lightSensorDownIntensity > -rangeLightIntensity)) LedWrite(LED_G,ON);
	else if(lightSensorUpIntensity > lightSensorDownIntensity) {angleVertical -= 1; LedWrite(LED_G,TOG);}
	else {angleVertical += 1; LedWrite(LED_G,TOG);}

	// Limits
	if(angleVertical > 90) angleVertical = 90;
	else if(angleVertical < 0) angleVertical = 0;
	if(angleHorizontal > 180) angleHorizontal = 180;
	else if(angleHorizontal < 0) angleHorizontal = 0;
}

void AlgorithmSunCalculatePosition()
{
	// Calculate Sun position
	SunCalculatePosition(timezone,dayYear(day,month),hr,mn,sc,LONGITUDE,LATITUDE);

	// Assign value
	angleHorizontal = SunGetCurrentAzimuth() - 90.0;	// -90 -> function return angle from N, device has max range from E to W
	angleVertical = SunGetCurrentZenith();

}

unsigned char charToNum(unsigned char sign)
{
	return (sign - 48);
}
