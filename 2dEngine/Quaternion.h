#pragma once

#include "Vector3.h"

namespace eg
{
	struct Quaternion
	{
		float w = 0.0f;
		Vector3f v = { 0.0f, 0.0f, 0.0f };
	public:
		Quaternion() = default;
		Quaternion(float w, const Vector3f& v);
		Quaternion(const Vector3f& axis, float angle);
		Quaternion(float rotX, float rotY, float rotZ);
		Quaternion operator+(const Quaternion& rhs) const;
		Quaternion& operator+=(const Quaternion& rhs);
		Quaternion operator-(const Quaternion& rhs) const;
		Quaternion& operator-=(const Quaternion& rhs);
		Quaternion operator*(float rhs) const;
		Quaternion& operator*=(float rhs);
		Quaternion operator/(float rhs) const;
		Quaternion& operator/=(float rhs);
		Quaternion operator-() const;
		Quaternion operator*(const Quaternion& rhs) const;
		Quaternion& operator*=(const Quaternion& rhs);
		float dotProduct(const Quaternion& rhs) const;
		float getLengthSq() const;
		float getLength() const;
		Quaternion& normalize();
		Quaternion getNormalized() const;
		bool operator==(const Quaternion& rhs) const;
		bool operator!=(const Quaternion& rhs) const;
		Quaternion getAxisAngle() const;
	};
}