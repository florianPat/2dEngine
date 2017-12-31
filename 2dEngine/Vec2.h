#pragma once

#include <math.h>

namespace eg
{
	template<typename T>
	struct Vec2
	{
		T x, y;
		Vec2() = default;
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2 operator+(const Vec2<T>& rhs) const
		{
			return Vec2(x + rhs.x, y + rhs.y);
		}
		Vec2& operator+=(const Vec2<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vec2 operator-(const Vec2<T>& rhs) const
		{
			return Vec2(x - rhs.x, y - rhs.y);
		}
		Vec2& operator-=(const Vec2<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vec2 operator*(const T rhs) const
		{
			return Vec2(x * rhs, y * rhs);
		}
		Vec2& operator*=(const T rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		T operator*(const Vec2<T> rhs)
		{
			return x * rhs.x + y * rhs.y;
		}
		Vec2 operator-() const
		{
			return Vec2{ -x, -y };
		}
		T getLenght() const
		{
			return (T)sqrtf((float)getLenghtSq());
		}
		T getLenghtSq() const
		{
			return x * x + y * y;
		}
		Vec2& normalize()
		{
			T length = getLenght();
			x /= length;
			y /= length;
			return *this;
		}
		Vec2 getNormalized() const
		{
			T lenght = getLenght();
			return Vec2(x / lenght, y / lenght);
		}
	};

	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vei2;
}