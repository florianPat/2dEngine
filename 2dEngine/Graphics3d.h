#pragma once

#include "Graphics2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Mat4x4.h"
#include "Plane.h"

namespace eg
{
	enum State
	{
		ACTIVE = 1,
		CLIPPED = 2,
		BACKFACE = 4,
	};

	struct Vertex
	{
		Vector4f pos;
		Vector3f normal;
		Vector2f textureCoord;
		Color color = Colors::White;
		Color shadedColor;
	};

	struct Polygon
	{
		Vertex localCoords[3];
		Vertex transformedCoords[3];
		uint32_t state = State::ACTIVE;
		uint32_t materialIndex = 0;
	};

	struct Material
	{
		Color emissiveColor;
		float ambient, diffuse, specular, specularPower;
		Texture texture;
		Color reflectivityA, reflectivityD, reflectivityS;

		void computeReflectivities();
	};

	enum class LightType
	{
		AMBIENT,
		DIRECTIONAL,
		POINT,
		EMISSIVE,
		SPOT,
	};

	struct Light
	{
		LightType lightType;

		Color ambientIntensity;
		Color diffuseIntensity;
		Color specularIntensity;

		Vector3f pos;
		Vector3f dir;

		float attenuationConstant, attentuationLinear, attenuationQuadratic;
		float spotInner, spotOuter, spotPower;

		bool active = true;
	};

	namespace effects
	{
		Color constColorPixelShader(const Vertex& vertex, const Material& material);
	}

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
		uint32_t nAddedClippingPolygons = 0;
		Polygon polygons[265];
		void transform(const Mat4x4& transform, TransformCase transformCase);
		void modelToWorldTranslation(TransformCase transformCase = TransformCase::TRANSFORM_COORDS_ONLY);
		void modelToWorldRotation(TransformCase transformCase = TransformCase::TRANSFORM_COORDS_ONLY);
		void modelToWorldScale(TransformCase transformCase = TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS);
		float computeMaxRadius() const;
		void doZDivide();
		void cullBackfaces(const Vector3f cameraWorldPos);
		void clearFlags();
		void drawWireframe(Graphics2d& gfx) const;
		void drawSolid(Graphics2d& gfx, const std::vector<Material>& materials) const;
		void clipInCameraSpace(float fov, float nearZ, float farZ);
		void removeAddedClippingPolygons();
		void doConstantShading();
		void doFlatShading(const std::vector<Light>& lights);
		void doGouradShading(const std::vector<Light>& lights);
	private:
		Vector3f clipLineToNearPlane(const Vertex* coords, float zNear, uint32_t p0Index, uint32_t p1Index, const Plane& plane);
		Vector4f computeRotation(const Vector4f& p) const;
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