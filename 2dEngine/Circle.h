#pragma once

#include "Vector2.h"
#include "Color.h"

namespace eg
{
	struct FloatCircle
	{
		float radius;
		Vector2f center;
		Color color;

		FloatCircle() = default;
		FloatCircle(const Vector2f& center, float radius, Color c = Color());
		FloatCircle(float centerX, float centerY, float radius, Color c = Color());
		bool intersects(const FloatCircle& other) const;
	};
}