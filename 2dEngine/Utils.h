#pragma once

#include "globalDefs.h"
#include "Vector2.h"
#include "Vector3.h"

static constexpr float PIf = 3.1415927f;

static constexpr float PiOver180 = PIf / 180;
static constexpr float _180OverPi = 180 / PIf;

namespace utils
{
	uint32_t getGUID();
	float degreesToRadians(float degree);
	float radiansToDegrees(float radians);
	eg::Vector2f polarToCartesian(const eg::Vector3f& pol);
	eg::Vector2f cartesianToPolar(const eg::Vector2f& cart);
	eg::Vector3f cylindricalToCartesian(const eg::Vector3f& cylin);
	eg::Vector3f cartesianToCylindrical(const eg::Vector3f& cart);
	eg::Vector3f sphericalToCartesian(const eg::Vector3f& speh);
	eg::Vector3f cartesianToSpherical(const eg::Vector3f& cart);
}