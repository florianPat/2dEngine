#pragma once

#include "Vec2.h"
#include "Color.h"

namespace eg
{
	struct FloatCircle
	{
		float radius;
		Vec2f center;
		Color color;

		FloatCircle() = default;
		FloatCircle(const Vec2f& center, float radius, Color c = Color());
		FloatCircle(float centerX, float centerY, float radius, Color c = Color());
		bool intersects(const FloatCircle& other) const;
	};
}