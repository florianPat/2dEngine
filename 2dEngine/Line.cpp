#include "Line.h"

namespace eg
{
	std::optional<Vector2f> Line::getIntersectionDeltas(const Line& other) const
	{
		const Vector3f v0 = p1 - p0;
		float detA = v0.x * (-v0.y) - (-v0.x) * v0.y;

		// NOTE: Use cramers rule to solve the system of setting the two parametric plane equations, divied into their components,
		// equal to one another
		float t0 = ((other.p0.x - p0.x) * (-v0.y) - (-v0.x) * (other.p0.y - p0.y)) / detA;
		float t1 = (v0.x * (other.p0.y - p0.y) - (other.p1.x - p0.x) * v0.y) / detA;
		if (t0 < 0.0f || t0 > 1.0f)
			return std::optional<Vector2f>();
		else if (t1 < 0.0f || t1 > 1.0f)
			return std::optional<Vector2f>();
		else
			return Vector2f{ t0, t1 };
	}
	std::optional<Vector3f> Line::getIntersectionPoint(const Line& other) const
	{
		const std::optional<Vector2f> intersectionDeltas = getIntersectionDeltas(other);
		if(!intersectionDeltas.has_value())
			return std::optional<Vector3f>();

		float t0 = intersectionDeltas->x;
		return Vector3f{ p0.x + p1.x * t0, p0.y + p1.y * t0, p0.z + p1.z * t0 };
	}
	std::optional<float> Line::getIntersectionDelta(const Plane& plane) const
	{
		const Vector3f v = p1 - p0;
		float d = -plane.normal.x * plane.point.x - plane.normal.y * plane.point.y - plane.normal.z * plane.point.z;
		float t = -(plane.normal.x * p0.x + plane.normal.y * p0.y + plane.normal.z * p0.z + d) /
			(plane.normal.x * v.x + plane.normal.y * v.y + plane.normal.z * v.z);
		if(t < 0.0f || t > 1.0f)
			return std::optional<float>();

		return t;
	}
	std::optional<Vector3f> Line::getIntersectionPoint(const Plane& other) const
	{
		auto intersectionDelta = getIntersectionDelta(other);
		if (!intersectionDelta.has_value())
			return std::optional<Vector3f>();

		return Vector3f{ p0.x + p1.x * (*intersectionDelta), p0.y + p1.y * (*intersectionDelta), p0.z + p1.z * (*intersectionDelta) };
	}
}
