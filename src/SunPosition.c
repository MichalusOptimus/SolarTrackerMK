/*
 * SunPosition.c
 *
 *  Created on: 07.03.2021
 *      Author: Michal
 */

#include "SunPosition.h"
#include "math.h"

// convert degree to radians and radians to degree
#define deg2rad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define rad2deg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

// azimuth and zenith angles
float solarAzimuthAngleDeg, solarZenithAngleDeg;

// calculate sun position by means of the time and the location of observer
void SunCalculatePosition(char timezone,short dayOfYear, unsigned char hr, unsigned char mn, unsigned char sc,float longitude, float latitude)
{
	// calculate fractional year
	float y = ((2*M_PI)/365.0)*((float)dayOfYear - 1.0 + (((float)hr - 12.0)/24.0));

	//estimate the equation of time (in minutes)
	float eqtime = 229.18*(0.000075+0.001868*cosf(y)-0.032077*sinf(y) - 0.014615*cosf(2*y)-0.040849*sinf(2*y));

	// solar declination angle
	float declinationRad = 0.006918 - 0.399912*cosf(y) + 0.070257*sinf(y) - 0.006758*cosf(2*y) + 0.000907*sinf(2*y) - 0.002697*cosf(3*y) + 0.00148*sinf(3*y);
	//float declinationDeg = rad2deg(declinationRad);

	// calculate time offset
	float time_offset = eqtime + 4*longitude - 60*(float)timezone;

	// true solar time
	float tst = (float)hr*60.0 + mn + sc/60 + time_offset;

	// solar hour angle
	float sha = (tst/4) - 180;

	// solar zenith angle
	float solarZenithAngle = acosf(sinf(deg2rad(latitude))*sinf(declinationRad)+cosf(deg2rad(latitude))*cosf(declinationRad)*cosf(deg2rad(sha)));
	solarZenithAngleDeg = rad2deg(solarZenithAngle);

	//solar azimuth angle from North
	float solarAzimuthAngle = acosf((sinf(deg2rad(latitude))*cosf(solarZenithAngle)-sinf(declinationRad))/(cosf(deg2rad(latitude))*sinf(solarZenithAngle)));
	if(sha>0)	solarAzimuthAngleDeg = fmod(rad2deg(solarAzimuthAngle)+180,360);
	else solarAzimuthAngleDeg = fmod(540 - rad2deg(solarAzimuthAngle),360);
}

// return current azimuth angle
float SunGetCurrentAzimuth()
{
	return solarAzimuthAngleDeg;
}

// return current zenith angle
float SunGetCurrentZenith()
{
	return solarZenithAngleDeg;
}

