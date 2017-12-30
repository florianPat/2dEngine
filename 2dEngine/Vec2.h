#pragma once

#include <math.h>

namespace eg
{
	template<typename T>
	struct Vec2_
	{
		T x, y;
		Vec2_() = default;
		Vec2_(T x, T y) : x(x), y(y) {}
		Vec2_ operator+(const Vec2_<T>& rhs) const
		{
			return Vec2_(x + rhs.x, y + rhs.y);
		}
		Vec2_& operator+=(const Vec2_<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vec2_ operator-(const Vec2_<T>& rhs) const
		{
			return Vec2_(x - rhs.x, y - rhs.y);
		}
		Vec2_& operator-=(const Vec2_<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vec2_ operator*(const T rhs) const
		{
			return Vec2_(x * rhs, y * rhs);
		}
		Vec2_& operator*=(const T rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		T getLenght() const
		{
			return (T)sqrtf((float)getLenghtSq());
		}
		T getLenghtSq() const
		{
			return x * x + y * y;
		}
		Vec2_& normalize()
		{
			T length = getLenght();
			x /= length;
			y /= length;
			return *this;
		}
		Vec2_ getNormalized() const
		{
			T lenght = getLenght();
			return Vec2_(x / lenght, y / lenght);
		}
	};

	typedef Vec2_<float> Vec2;
	typedef Vec2_<int> Vei2;
}