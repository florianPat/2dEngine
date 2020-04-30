#pragma once

#include "Mat4x4.h"

namespace eg
{
	class Camera
	{
		Mat4x4 worldToView;
		bool updated = false;
	public:
		Vector3f dir;
		Vector3f pos;
	public:
		Camera() = default;
		Camera(const Vector3f& dir, Vector3f pos);
		bool hasUpdated();
		void update();
		const Mat4x4& getWorldToViewMatrix() const;
	};
}