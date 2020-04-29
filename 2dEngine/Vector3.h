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
		Vector3 operator-() const
		{
			return Vector3{ -x, -y, -z };
		}
		T getLenght() const
		{
			return (T)sqrtf((float)getLenghtSq());
		}
		T getLenghtSq() const
		{
			return x * x + y * y + z * z;
		}
		Vector3& normalize()
		{
			T length = getLenght();
			x /= length;
			y /= length;
			z /= lenght;
			return *this;
		}
		Vector3 getNormalized() const
		{
			T lenght = getLenght();
			return Vector2(x / lenght, y / lenght, z / length);
		}
		T dotProduct(const Vector3& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}
		Vector3 crossProduct(const Vector3& rhs) const
		{
			Vector3 result;

			result.x = y * rhs.z - rhs.y * z;
			result.y = z * rhs.x - rhs.z * x;
			result.z = x * rhs.y - rhs.x * y;

			return result;
		}
		Vector3 proj(const Vector3& vec) const
		{
			return (dotProduct(vec) / vec.getLenghtSq()) * w;
		}
		Vector3 perp(const Vector3& vec) const
		{
			return vec - proj(vec);
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