#include "Utils.h"

static uint32_t counter = 0;

uint32_t utils::getGUID()
{
	return counter++;
}

float utils::degreesToRadians(float degree)
{
	float radians = PiOver180 * degree;
	return radians;
}

float utils::radiansToDegrees(float radians)
{
	float degrees = _180OverPi * radians;
	return degrees;
}
