#pragma once

namespace eg
{
	template <typename T>
	struct Vector3
	{
		T x, y, z;
		Vector3() = default;
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3 operator+(const Vector3<T>& rhs) const
		{
			return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
		}
		Vector3& operator+=(const Vector3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}
		Vector3 operator-(const Vector3<T>& rhs) const
		{
			return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
		}
		Vector3& operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}
		Vector3& operator*=(const T &rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}
		Vector3	operator*(const T &rhs) const
		{
			return Vector3(*this) *= rhs;
		}
		Vector3& operator*=(const Vector3& rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}
		Vector3 operator*(const Vector3& rhs) const
		{
			return Vector3(*this) *= rhs;
		}
		T dotProduct(const Vector3& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		Vector3& operator/=(const T &rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
			return *this;
		}
		Vector3 operator/(const T &rhs) const
		{
			return Vector3(*this) /= rhs;
		}
		Vector3& operator/=(const Vector3& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
			return *this;
		}
		Vector3 operator/(const Vector3& rhs) const
		{
			return Vector3(*this) /= rhs;
		}
		bool operator==(const Vector3 &rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}
		bool operator!=(const Vector3 &rhs) const
		{
			return !(*this == rhs);
		}
	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<int> Vector3i;
}