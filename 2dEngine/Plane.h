#pragma once

#include "Vector3.h"
#include <optional>

namespace eg
{
	struct Plane
	{
		Vector3f point;
		Vector3f normal;

		float computeHalfspace(const Vector3f pointIn);
	};
}