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

eg::Vector2f utils::polarToCartesian(const eg::Vector3f& pol)
{
	eg::Vector2f result;

	result.x = pol.x * cosf(pol.y);
	result.y = pol.x * sinf(pol.y);

	return result;
}

eg::Vector2f utils::cartesianToPolar(const eg::Vector2f& cart)
{
	eg::Vector2f result;

	result.x = cart.getLenght();
	result.y = atan2f(cart.y, cart.x);

	return result;
}

eg::Vector3f utils::cylindricalToCartesian(const eg::Vector3f& cylin)
{
	eg::Vector3f result;

	result.x = cylin.x * cosf(cylin.y);
	result.y = cylin.x * sinf(cylin.y);
	result.z = cylin.z;

	return result;
}

eg::Vector3f utils::cartesianToCylindrical(const eg::Vector3f& cart)
{
	eg::Vector3f result;

	result.x = cart.getLenght();
	result.y = atan2f(cart.y, cart.x);
	result.z = cart.z;

	return result;
}

eg::Vector3f utils::sphericalToCartesian(const eg::Vector3f& speh)
{
	eg::Vector3f result;

	result.x = speh.x * cosf(speh.y) * sinf(speh.z);
	result.y = speh.x * sinf(speh.y) * sinf(speh.z);
	result.z = cosf(speh.z);

	return result;
}

eg::Vector3f utils::cartesianToSpherical(const eg::Vector3f& cart)
{
	eg::Vector3f result;
	
	result.x = cart.getLenght();
	result.y = atan2f(cart.y, cart.x);
	result.z = acosf(cart.z / result.x);

	return result;
}
