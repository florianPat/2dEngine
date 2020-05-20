#include "Plane.h"

float eg::Plane::computeHalfspace(const Vector3f pointIn)
{
	return normal.dotProduct(pointIn - point);
}
