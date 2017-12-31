#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Color.h"

namespace eg
{
	class ORectF
	{
	private:
		friend class Graphics2d;

		static constexpr float PI = 3.1415927f;

		float angle;
		Vec2f xAxis, yAxis;
	public:
		float width, height;
		Vec2f pos;
		Vec2f origin;
		Color color;
	public:
		//angle has to be in degrees!
		//Local origin
		ORectF(float left, float top, float width, float height, float angle, Vec2f& origin = Vec2f(0.0f, 0.0f), Color c = Color());
		ORectF(Vec2f& topLeft, float width, float height, float angle, Vec2f& origin = Vec2f(0.0f, 0.0f), Color c = Color());
		explicit ORectF(const RectF& other);
		//angle has to be in degrees!
		void setAngle(float newAngle);
		float getAngle() const;
		bool intersects(const ORectF& other) const;
		//TODO: Add intersects Circle
		//TODO: Add collide to all shapes!
	private:
		void getPointsAxis(Vec2f* points, Vec2f* axis) const;
		Vec2f getProjectionMinMax(const Vec2f* points, const Vec2f& axis, bool isXAxis) const;
	};
}