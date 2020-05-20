#pragma once

#include "Vector3.h"
#include <optional>
#include "Vector2.h"
#include "Plane.h"

namespace eg
{
	struct Line
	{
		Vector3f p0;
		Vector3f p1;

		std::optional<Vector2f> getIntersectionDeltas(const Line& other) const;
		std::optional<Vector3f> getIntersectionPoint(const Line& other) const;
		std::optional<float> getIntersectionDelta(const Plane& plane) const;
		std::optional<Vector3f> getIntersectionPoint(const Plane& other) const;
	};
}