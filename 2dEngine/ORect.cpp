#include "ORect.h"

namespace eg
{
	//NOTE: angle from degrees in radians, because cosf uses radians, but in matrix of SFML in Shape it uses degrees, so you have to convert back and forth...
	ORectF::ORectF(float left, float top, float width, float height, float angle, Vector2f& origin, Color c) : angle(angle*PI / 180), pos(Vector2f{ left, top }), width(width), height(height),
		xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
		origin(origin), color(c)
	{
	}

	ORectF::ORectF(Vector2f & topLeft, float width, float height, float angle, Vector2f& origin, Color c) : angle(angle*PI / 180), pos(topLeft), width(width), height(height),
		xAxis(cosf(this->angle), sinf(this->angle)), yAxis((-sinf(this->angle)), cosf(this->angle)),
		origin(origin), color(c)
	{
	}

	ORectF::ORectF(const RectF & other) : ORectF(Vector2f{ other.left, other.top }, other.right - other.left, other.bottom - other.top, 0.0f, Vector2f{ 0.0f, 0.0f }, other.color)
	{
	}

	void ORectF::setAngle(float newAngle)
	{
		angle = newAngle * PI / 180;
		xAxis = Vector2f(cosf(angle), sinf(angle));
		yAxis = Vector2f(-sinf(angle), cosf(angle));
	}

	void ORectF::getPointsAxis(Vector2f * points, Vector2f * axis) const
	{
		points[0] = { pos };
		points[1] = { pos.x + width, pos.y };
		points[2] = { pos.x + width, pos.y + height };
		points[3] = { pos.x, pos.y + height };

		//Global origin
		Vector2f origin = pos + this->origin;

		for (int i = 0; i < 4; ++i)
		{
			points[i] = Vector2f(pos + xAxis * (points[i].x - origin.x) + yAxis * (points[i].y - origin.y));
		}

		axis[0] = xAxis;
		axis[1] = yAxis;
	}

	Vector2f ORectF::getProjectionMinMax(const Vector2f * points, const Vector2f & axis, bool isXAxis) const
	{
		Vector2f result = { points[0].x * axis.x + points[0].y * axis.y, points[0].x * axis.x + points[0].y * axis.y };

		for (int i = 1; i < 4; ++i)
		{
			float proj = points[i].x * axis.x + points[i].y * axis.y;

			if (proj < result.x)
				result.x = proj;
			else if (proj > result.y)
				result.y = proj;
		}

		return result;
	}

	float ORectF::getAngle() const
	{
		return angle * 180 / PI;
	}
	bool ORectF::intersects(const ORectF & other) const
	{
		Vector2f axis[4] = {};

		Vector2f s1Points[4] = {};
		Vector2f s2Points[4] = {};

		getPointsAxis(s1Points, axis);
		other.getPointsAxis(s2Points, axis + 2);

		for (int i = 0; i < 4; ++i)
		{
			Vector2f s1MinMax = getProjectionMinMax(s1Points, axis[i], i % 2 == 0);
			Vector2f s2MinMax = getProjectionMinMax(s2Points, axis[i], i % 2 == 0);

			if ((s2MinMax.x > s1MinMax.y || s2MinMax.y < s1MinMax.x))
				return false;
			else
			{
				continue;
			}
		}

		return true;
	}
}