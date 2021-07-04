/*
 * SunPosition.h
 *
 *  Created on: 07.03.2021
 *      Author: Michal
 */



float SunGetCurrentAzimuth();

float SunGetCurrentZenith();

void SunCalculatePosition(char timezone,short dayOfYear, unsigned char hr, unsigned char mn, unsigned char sc,float longitude, float latitude);
