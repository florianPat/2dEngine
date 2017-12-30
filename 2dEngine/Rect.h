#pragma once

#include "Color.h"
#include "Vec2.h"

namespace eg
{
	template<typename T>
	struct Rect
	{
		T left, right, top, bottom;
		Color color;
		Rect() = default;
		Rect(T left, T top, T right, T bottom, Color c = Color()) : left(left), right(right), top(top), bottom(bottom), color(c) {}
		Rect(Vec2<T> topLeft, T width, T height, Color c = Color()) : left(topLeft.x), top(topLeft.y), right(topLeft.x + width), bottom(topLeft.y + height), color(c) {}
		bool intersects(const Rect<T>& other) const
		{
			return (left < other.right && right > other.left &&	top < other.bottom && bottom > other.top);
		}
	};

	typedef Rect<float> RectF;
	typedef Rect<int> RectI;
}