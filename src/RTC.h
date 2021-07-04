/*
 * RTC.h
 *
 *  Created on: 04.05.2021
 *      Author: Michal
 */

// Initialization RTC
void RTC_Init();

/* Set time
 Parameters: 	hr - hours 0 - 23
				mn - minutes 0 - 59
				sc - seconds 0 - 59*/
void RTC_SetTime(unsigned char hr, unsigned char mn, unsigned char sc);

/* Set date
 Parameters: 	day 0 - 31
				month 0 - 12
				year 0 - 99 */
void RTC_SetDate(unsigned char day, unsigned char month, unsigned char year);

// Reading time from RTC
void RTC_ReadTime(unsigned char *hr, unsigned char *mn, unsigned char *sc);

// Reading date from RTC
void RTC_ReadDate(unsigned char *day, unsigned char *month, unsigned char *year);

// Calculate the day number of the year
short dayYear(char day, char month);
