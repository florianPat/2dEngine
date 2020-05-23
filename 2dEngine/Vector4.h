#pragma once

#include "Vector3.h"
#include "Vector2.h"

namespace eg
{
	template <typename T>
	struct Vector4 : public Vector3<T>
	{
		T w = 0;
		Vector4() = default;
		Vector4(T x, T y, T z, T w) : Vector3(x, y, z), w(w) {}
		Vector4(const Vector3& other) : Vector3(other), w(1.0f) {}
		void homogeneousDivide()
		{
			x /= w;
			y /= w;
			z /= w;
			w = 1.0f;
		}
		template <typename O>
		operator Vector2<O>() const
		{
			return { (O)x, (O)y };
		}
		Vector4 operator+(const Vector3<T>& rhs) const
		{
			return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w);
		}
		T dotProduct(const Vector4& rhs) const
		{
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}
	};

	typedef Vector4<float> Vector4f;
	typedef Vector3<int32_t> Vector4i;
}