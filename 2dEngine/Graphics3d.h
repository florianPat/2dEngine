#pragma once

#include "Graphics2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Mat4x4.h"
#include "Plane.h"

namespace eg
{
	enum class ShadingMode
	{
		CONST_COLOR,
		FLAT,
		GOURAUD,
		PHONG,
	};

	enum State
	{
		ACTIVE = 1,
		CLIPPED = 2,
		BACKFACE = 4,
	};

	struct Polygon
	{
		Vector4f localCoords[3];
		Vector4f transformedCoords[3];
		Color color = Colors::White;
		ShadingMode shadingMode = ShadingMode::CONST_COLOR;
		uint32_t state = State::ACTIVE;
	};

	enum class TransformCase
	{
		LOCAL_COORDS_ONLY,
		TRANSFORM_COORDS_ONLY,
		LOCAL_COORDS_TO_TRANSFORM_COORDS,
	};

	struct Object
	{
		Vector3f worldPos;
		Vector3f rot;
		Vector3f scl;
		Vector3f basis[3] = { {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} };
		uint32_t nPolygons;
		Polygon polygons[265];
		void transform(const Mat4x4& transform, TransformCase transformCase, bool transformBasis);
		void modelToWorldTranslation(TransformCase transformCase = TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS);
		float computeMaxRadius() const;
		void doZDivide();
		void cullBackfaces(const Vector3f cameraWorldPos);
		void clearFlags();
		void drawWireframe(Graphics2d& gfx) const;
		void drawSolid(Graphics2d& gfx) const;
	};

	struct Camera
	{
		Vector3f worldPos;
		Vector3f direction;

		Vector3f target;
		Vector3f lookAt, up, right; // NOTE: z, y, x basis for new camera coord system

		float fov;
		static constexpr float viewplaneWidth = 2.0f;
		static constexpr float viewplaneHeight = 2.0f;

		float nearClippingPlane;
		float farClippingPlane;
		Plane rightClippingPlane;
		Plane leftClippingPlane;
		Plane topClippingPlane;
		Plane bottomClippingPlane;

		float viewportWidth;
		float viewportHeight;
		float aspectRatio;

		Mat4x4 worldToCameraTransform;
		Mat4x4 cameraToPerspectiveTransform;
		Mat4x4 perspectiveToScreenTransform;
	private:
		Camera(const Vector3f& worldPos, float nearClippingPlane, float farClippingPlane, float fov, float viewportWidth,
			float viewportHeight);
	public:
		Camera(const Vector3f& worldPos, const Vector3f& targetIn, float nearClippingPlane, float farClippingPlane,
			float fov, float viewportWidth, float viewportHeight);
		Camera(const Vector3f& worldPos, float nearClippingPlane, float
			farClippingPlane, const Vector3f& directionIn, float fov, float viewportWidth, float viewportHeight);
	};
}