#include "Circle.h"
#include <math.h>

eg::FloatCircle::FloatCircle(const Vec2f & center, float radius, Color c) : center(center), radius(radius), color(c)
{
}

eg::FloatCircle::FloatCircle(float centerX, float centerY, float radius, Color c) : center(centerX, centerY), radius(radius), color(c)
{
}

bool eg::FloatCircle::intersects(const FloatCircle & other) const
{
	Vec2f vec = center - other.center;
	if (sqrtf(vec.x * vec.x + vec.y * vec.y) < radius + other.radius)
		return true;
	else
		return false;
}
