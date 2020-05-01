#include "Quaternion.h"
#include "Utils.h"

eg::Quaternion::Quaternion(float w, const Vector3f& v) : w(w), v(v)
{
}

eg::Quaternion::Quaternion(const Vector3f& axis, float angle) : w(cosf(utils::degreesToRadians(angle / 2.0f))),
																v(axis * sinf(utils::degreesToRadians(angle / 2.0f)))
{
}

eg::Quaternion::Quaternion(float rotX, float rotY, float rotZ)
{
	Quaternion xAxis({ 1.0f, 0.0f, 0.0f }, rotX);
	Quaternion yAxis({ 0.0f, 1.0f, 0.0f }, rotY);
	Quaternion zAxis({ 0.0f, 0.0f, 1.0f }, rotZ);

	*this = xAxis * yAxis * zAxis;
}

eg::Quaternion eg::Quaternion::operator+(const Quaternion& rhs) const
{
	return Quaternion{ w + rhs.w, v + rhs.v };
}

eg::Quaternion& eg::Quaternion::operator+=(const Quaternion& rhs)
{
	w += rhs.w;
	v += rhs.v;

	return *this;
}

eg::Quaternion eg::Quaternion::operator-(const Quaternion& rhs) const
{
	return Quaternion(w - rhs.w, v - rhs.v);
}

eg::Quaternion& eg::Quaternion::operator-=(const Quaternion& rhs)
{
	w -= rhs.w;
	v -= rhs.v;

	return *this;
}

eg::Quaternion eg::Quaternion::operator*(float rhs) const
{
	return Quaternion(w * rhs, v * rhs);
}

eg::Quaternion& eg::Quaternion::operator*=(float rhs)
{
	w *= rhs;
	v *= rhs;

	return *this;
}

eg::Quaternion eg::Quaternion::operator/(float rhs) const
{
	return Quaternion(w / rhs, v / rhs);
}

eg::Quaternion& eg::Quaternion::operator/=(float rhs)
{
	w /= rhs;
	v /= rhs;

	return *this;
}

eg::Quaternion eg::Quaternion::operator-() const
{
	return Quaternion(w, -v);
}

eg::Quaternion eg::Quaternion::operator*(const Quaternion& rhs) const
{
	return Quaternion(w * rhs.w - v.dotProduct(rhs.v), rhs.v * w + v * rhs.w + rhs.v.crossProduct(v));
}

float eg::Quaternion::dotProduct(const Quaternion& rhs) const
{
	return w * rhs.w + v.dotProduct(rhs.v);
}

float eg::Quaternion::getLengthSq() const
{
	return w * w + v.getLenghtSq();
}

float eg::Quaternion::getLength() const
{
	return sqrtf(getLengthSq());
}

eg::Quaternion& eg::Quaternion::normalize()
{
	*this /= getLength();

	return *this;
}

eg::Quaternion eg::Quaternion::getNormalized() const
{
	float length = getLength();
	return Quaternion(w / length, v / length);
}

bool eg::Quaternion::operator==(const Quaternion& rhs) const
{
	return (w == rhs.w && v == rhs.v);
}

bool eg::Quaternion::operator!=(const Quaternion& rhs) const
{
	return !(*this == rhs);
}

eg::Quaternion eg::Quaternion::getAxisAngle() const
{
	return Quaternion(2 * acosf(w), v / (sqrtf(1 - (w * w))));
}
