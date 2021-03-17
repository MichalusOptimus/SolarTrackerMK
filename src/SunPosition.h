/*
 * SunPosition.h
 *
 *  Created on: 07.03.2021
 *      Author: Michal
 */



float SunGetCurrentAzimuth();

float SunGetCurrentZenith();

void SunCalculatePosition(unsigned char timezone,int dayOfYear, unsigned char hr, unsigned char mn, unsigned char sc,float longitude, float latitude);
