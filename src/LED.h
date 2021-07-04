/*
 * LED.h
 *
 *	LED_MAN -  PA0 - manual mode
 *	LED_AUTO - PA2 - automatic mode
 */


typedef enum {ON = 1, OFF = 0, TOG = 2} LedState;


typedef enum {	LED2 = 0x00000080,			// LED2 on PB7 (built-in Nucleo)
				LED3 = 0x00004000,			// LED3 on PB 14 (built-in Nucleo)
				LED_MAN = 0x00000001,		// LED_MAN on PA0 -> manual mode LED
				LED_G = 0x00000002,			// LED_G on PA1 -> green color RGB
				LED_AUTO = 0x00000004		// LED_AUTO on PA2 -> automatic mode LED
				} LedPin;


// Configure GPIO
void LedInit();

// Write LED state
// Parameters: 	LED pin (LED2, LED3, LED_R, LED_G, LED_B
//				state (ON, OFF, TOG -> toggle)
void LedWrite(LedPin pin, LedState state);




