#pragma once

#include "Vector2.h"
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
		Vector2f xAxis, yAxis;
	public:
		float width, height;
		Vector2f pos;
		Vector2f origin;
		Color color;
	public:
		//angle has to be in degrees!
		//Local origin
		ORectF(float left, float top, float width, float height, float angle, Vector2f& origin = Vector2f(0.0f, 0.0f), Color c = Color());
		ORectF(Vector2f& topLeft, float width, float height, float angle, Vector2f& origin = Vector2f(0.0f, 0.0f), Color c = Color());
		explicit ORectF(const RectF& other);
		//angle has to be in degrees!
		void setAngle(float newAngle);
		float getAngle() const;
		bool intersects(const ORectF& other) const;
		//TODO: Add intersects Circle
		//TODO: Add collide to all shapes!
	private:
		void getPointsAxis(Vector2f* points, Vector2f* axis) const;
		Vector2f getProjectionMinMax(const Vector2f* points, const Vector2f& axis, bool isXAxis) const;
	};
}