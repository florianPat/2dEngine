#include "Graphics3d.h"

namespace eg
{
	void Object::transform(const Mat4x4& transform, TransformCase transformCase, bool transformBasis)
	{
		switch (transformCase)
		{
			case TransformCase::LOCAL_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = polygons[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						polygons[i].localCoords[j] = transform * polygons[i].localCoords[j];
					}
				}
				break;
			}
			case TransformCase::TRANSFORM_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = polygons[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						polygons[i].transformedCoords[j] = transform * polygons[i].transformedCoords[j];
					}
				}
				break;
			}
			case TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = polygons[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						polygons[i].transformedCoords[j] = transform * polygons[i].localCoords[j];
					}
				}
				break;
			}
			default:
			{
				InvalidCodePath;
				break;
			}
		}

		if (transformBasis)
		{
			for (uint32_t i = 0; i < 3; ++i)
			{
				basis[i] = transform * basis[i];
			}
		}
	}
	void Object::modelToWorldTranslation(TransformCase transformCase)
	{
		switch (transformCase)
		{
			case TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = polygons[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						polygons[i].transformedCoords[j] = polygons[i].localCoords[j] + worldPos;
					}
				}
				break;
			}
			case TransformCase::TRANSFORM_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = polygons[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						polygons[i].transformedCoords[j] = polygons[i].transformedCoords[j] + worldPos;
					}
				}
				break;
			}
			default:
			{
				InvalidCodePath;
				break;
			}
		}
	}
	float Object::computeMaxRadius() const
	{
		float result = 0.0f;
		float currentLengthSq = 0.0f;

		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				currentLengthSq = polygons[i].localCoords[j].getLenghtSq();
				if (result < currentLengthSq)
				{
					result = currentLengthSq;
				}
			}
		}

		return result;
	}
	void Object::doZDivide()
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			for (uint32_t j = 0; j < 3; ++j)
			{
				polygons[i].transformedCoords[j].homogeneousDivide();
			}
		}
	}
	void Object::cullBackfaces(const Vector3f cameraWorldPos)
	{
		Vector3f normal;
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			Vector3f v0 = polygons[i].transformedCoords[1] - polygons[i].transformedCoords[0];
			Vector3f v1 = polygons[i].transformedCoords[2] - polygons[i].transformedCoords[0];
			normal = v0.crossProduct(v1);

			Vector3f polygonCameraDifference = polygons[i].transformedCoords[0] - cameraWorldPos;

			if (normal.dotProduct(polygonCameraDifference) > 0.0f)
			{
				polygons[i].state |= State::BACKFACE;
			}
		}
	}
	void Object::clearFlags()
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			polygons[i].state = State::ACTIVE;
		}
	}
	void Object::drawWireframe(Graphics2d& gfx) const
	{
		std::vector<eg::Vector2i> polyline;
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & eg::State::ACTIVE && !(polygons[i].state & eg::State::BACKFACE) &&
				!(polygons[i].state & eg::State::CLIPPED))
			{
				polyline.push_back(polygons[i].transformedCoords[0]);
				polyline.push_back(polygons[i].transformedCoords[1]);
				polyline.push_back(polygons[i].transformedCoords[2]);
				gfx.drawPolyline(polyline, polygons[i].color);
				polyline.clear();
			}
		}
	}
	void Object::drawSolid(Graphics2d& gfx) const
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & eg::State::ACTIVE && !(polygons[i].state & eg::State::BACKFACE) &&
				!(polygons[i].state & eg::State::CLIPPED))
			{
				switch (polygons[i].shadingMode)
				{
					case ShadingMode::CONST_COLOR:
					{
						gfx.drawTriangle(polygons[i].transformedCoords[0], polygons[i].transformedCoords[1],
							polygons[i].transformedCoords[2], polygons[i].color);
						break;
					}
					default:
					{
						InvalidCodePath;
						break;
					}
				}
			}
		}
	}
	Camera::Camera(const Vector3f& worldPos, float nearClippingPlane, float farClippingPlane, float fov, float viewportWidth,
		float viewportHeight)
		:	worldPos(worldPos), direction(), target(), lookAt(), up(), right(), fov(fov),
			nearClippingPlane(nearClippingPlane), farClippingPlane(farClippingPlane),
			rightClippingPlane(), leftClippingPlane(), topClippingPlane(), bottomClippingPlane(),
			viewportWidth(viewportWidth), viewportHeight(viewportHeight), aspectRatio(viewportWidth / viewportHeight)
	{
		float d = tanf(utils::degreesToRadians(fov / 2.0f)) * nearClippingPlane;
		// NOTE: The normal is the perpendicular vector of, when visualised in the appropiate plane, the  vector to the edge
		// of the view plane
		rightClippingPlane = { Vector3f{0.0f, 0.0f, 0.0f}, Vector3f{d, 0.0f, -1.0f} };
		leftClippingPlane = { Vector3f{0.0f, 0.0f, 0.0f}, Vector3f{d, 0.0f, 1.0f} };
		topClippingPlane = { Vector3f{0.0f, 0.0f, 0.0f}, Vector3f{0.0f, d, -1.0f} };
		bottomClippingPlane = { Vector3f{0.0f, 0.0f, 0.0f}, Vector3f{0.0f, d, 1.0f} };

		cameraToPerspectiveTransform = Mat4x4::persProj(nearClippingPlane, farClippingPlane, aspectRatio, fov);

		float halfViewportWidth = viewportWidth / 2.0f;
		float halfViewportHeight = viewportHeight / 2.0f;
		perspectiveToScreenTransform = Mat4x4::translate({halfViewportWidth, halfViewportHeight})
			* Mat4x4::scale({ halfViewportWidth, -halfViewportHeight });
	}
	Camera::Camera(const Vector3f& worldPos, const Vector3f& targetIn, float nearClippingPlane, float farClippingPlane, float fov,
		float viewportWidth, float viewportHeight)
		: Camera(worldPos, nearClippingPlane, farClippingPlane, fov, viewportWidth, viewportHeight)
	{
		// NOTE: Target can also be computed with spherical coords and the converted to cart, for easier visualisation of rotation for
		// first person camera
		target = targetIn;
		lookAt = target.getNormalized();
		up = { 0.0f, 1.0f, 0.0f };
		right = lookAt.crossProduct(up);
		up = lookAt.crossProduct(right);

		worldToCameraTransform = Mat4x4::identity();
		worldToCameraTransform.matrix[0 * 4 + 0] = right.x;
		worldToCameraTransform.matrix[1 * 4 + 0] = right.y;
		worldToCameraTransform.matrix[2 * 4 + 0] = right.z;
		worldToCameraTransform.matrix[0 * 4 + 1] = up.x;
		worldToCameraTransform.matrix[1 * 4 + 1] = up.y;
		worldToCameraTransform.matrix[2 * 4 + 1] = up.z;
		worldToCameraTransform.matrix[0 * 4 + 2] = lookAt.x;
		worldToCameraTransform.matrix[1 * 4 + 2] = lookAt.y;
		worldToCameraTransform.matrix[2 * 4 + 2] = lookAt.z;
		worldToCameraTransform *= Mat4x4::translate(-worldPos);
	}
	Camera::Camera(const Vector3f& worldPos, float nearClippingPlane, float farClippingPlane, const Vector3f& directionIn, float fov,
		float viewportWidth, float viewportHeight)
		:	Camera(worldPos, nearClippingPlane, farClippingPlane, fov, viewportWidth, viewportHeight)
	{
		direction = directionIn;

		worldToCameraTransform = Mat4x4::translate(-worldPos) * Mat4x4::rotateZ(-direction.z) * Mat4x4::rotateY(-direction.y) *
			Mat4x4::rotateX(-direction.x);
	}
}