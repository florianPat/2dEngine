#pragma once

#include <math.h>
#include "globalDefs.h"

namespace eg
{
	template<typename T>
	struct Vector2
	{
		T x = 0, y = 0;
		Vector2() = default;
		Vector2(T x, T y) : x(x), y(y) {}
		template <typename T2>
		explicit operator Vector2<T2>()
		{
			return{ (T2)x,(T2)y };
		}
		Vector2 operator+(const Vector2<T>& rhs) const
		{
			return Vector2(x + rhs.x, y + rhs.y);
		}
		Vector2& operator+=(const Vector2<T>& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}
		Vector2 operator-(const Vector2<T>& rhs) const
		{
			return Vector2(x - rhs.x, y - rhs.y);
		}
		Vector2& operator-=(const Vector2<T>& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}
		Vector2 operator*(const T rhs) const
		{
			return Vector2(x * rhs, y * rhs);
		}
		Vector2& operator*=(const T rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}
		T operator*(const Vector2<T> rhs)
		{
			return x * rhs.x + y * rhs.y;
		}
		Vector2 operator-() const
		{
			return Vector2{ -x, -y };
		}
		T getLenght() const
		{
			return (T)sqrtf((float)getLenghtSq());
		}
		T getLenghtSq() const
		{
			return x * x + y * y;
		}
		Vector2& normalize()
		{
			T length = getLenght();
			x /= length;
			y /= length;
			return *this;
		}
		Vector2 getNormalized() const
		{
			T lenght = getLenght();
			return Vector2(x / lenght, y / lenght);
		}
		T dotProduct(const Vector2& rhs) const
		{
			return x * rhs.x + y * rhs.y;
		}
		Vector2& operator/=(const T &rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}
		Vector2	operator/(const T &rhs) const
		{
			return Vector2(*this) /= rhs;
		}
		Vector2& operator/=(const Vector2& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}
		Vector2 operator/(const Vector2& rhs) const
		{
			return Vector2(*this) /= rhs;
		}
		bool operator==(const Vector2 &rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}
		bool operator!=(const Vector2 &rhs) const
		{
			return !(*this == rhs);
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;
}