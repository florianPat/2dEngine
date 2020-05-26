#include "Graphics3d.h"
#include "Line.h"

namespace eg
{
	void Object::transform(const Mat4x4& transform, TransformCase transformCase)
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
						polygons[i].localCoords[j].pos = transform * polygons[i].localCoords[j].pos;
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
						polygons[i].transformedCoords[j].pos = transform * polygons[i].transformedCoords[j].pos;
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
						polygons[i].transformedCoords[j].pos = transform * polygons[i].localCoords[j].pos;
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
						polygons[i].transformedCoords[j].pos = polygons[i].localCoords[j].pos + worldPos;
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
						polygons[i].transformedCoords[j].pos += worldPos;
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
	void Object::modelToWorldRotation(TransformCase transformCase)
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
						polygons[i].transformedCoords[j].pos = computeRotation(polygons[i].localCoords[j].pos);

						// NOTE: Also rotating the normal to transform that propery (no need to translate or scale)
						polygons[i].transformedCoords[j].normal = computeRotation(polygons[i].localCoords[j].normal);
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
						polygons[i].transformedCoords[j].pos = computeRotation(polygons[i].transformedCoords[j].pos);

						polygons[i].transformedCoords[j].normal = computeRotation(polygons[i].transformedCoords[j].normal);
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
	void Object::modelToWorldScale(TransformCase transformCase)
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
					polygons[i].transformedCoords[j].pos = polygons[i].localCoords[j].pos * scl;
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
					polygons[i].transformedCoords[j].pos *= scl;
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
				currentLengthSq = polygons[i].localCoords[j].pos.getLenghtSq();
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
				polygons[i].transformedCoords[j].pos.doZDivide();
			}
		}
	}
	void Object::cullBackfaces(const Vector3f cameraWorldPos)
	{
		Vector3f normal;
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			Vector3f v0 = polygons[i].transformedCoords[1].pos - polygons[i].transformedCoords[0].pos;
			Vector3f v1 = polygons[i].transformedCoords[2].pos - polygons[i].transformedCoords[0].pos;
			normal = v0.crossProduct(v1);

			Vector3f polygonCameraDifference = polygons[i].transformedCoords[0].pos - cameraWorldPos;

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
				polyline.push_back(polygons[i].transformedCoords[0].pos);
				polyline.push_back(polygons[i].transformedCoords[1].pos);
				polyline.push_back(polygons[i].transformedCoords[2].pos);
				gfx.drawPolyline(polyline, polygons[i].transformedCoords[0].color);
				polyline.clear();
			}
		}
	}
	void Object::drawSolid(Graphics2d& gfx, const std::vector<Material>& materials) const
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & eg::State::ACTIVE && !(polygons[i].state & eg::State::BACKFACE) &&
				!(polygons[i].state & eg::State::CLIPPED))
			{
				gfx.drawTriangle(polygons[i].transformedCoords,
					Delegate<Color(const Vertex&, const Material&)>::from <effects::constColorPixelShader>(),
					materials[polygons[i].materialIndex]);
			}
		}
	}
	void Object::clipInCameraSpace(float fov, float nearZ, float farZ)
	{
		float d = tanf(utils::degreesToRadians(fov / 2.0f));

		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & State::ACTIVE && !(polygons[i].state & State::BACKFACE))
			{
				polygons[i].state |= State::CLIPPED;
				uint32_t zNearClipping = 0; //1 for < nearZ for every vertex
				for (uint32_t j = 0; j < 3; ++j)
				{
					Vector3f coord = polygons[i].transformedCoords[j].pos;
					float tanZ = d * coord.z;
					if (coord.x < tanZ && coord.x > -tanZ && coord.y < tanZ && coord.y > -tanZ && coord.z > nearZ && coord.z < farZ)
					{
						polygons[i].state &= ~State::CLIPPED;
					}
					else if (coord.z < nearZ)
					{
						zNearClipping |= 1 << j;
					}
				}

				if (!(polygons[i].state & State::CLIPPED))
				{
					if (zNearClipping)
					{
						Plane plane{ {0.0f, 0.0f, nearZ }, {0.0f, 0.0f, 1.0f } };

						//TODO: Add vertex attributes clipping if I have vertex attributes!
						switch (zNearClipping)
						{
							case 1:
							{
								Vector3f intersection0 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 0, 1, plane);
								Vector3f intersection1 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 0, 2, plane);

								polygons[nPolygons].transformedCoords[0].pos = Vector4f(intersection0);
								polygons[nPolygons].transformedCoords[1] = polygons[i].transformedCoords[1];
								polygons[nPolygons].transformedCoords[2] = polygons[i].transformedCoords[2];
								++nPolygons;
								++nAddedClippingPolygons;

								polygons[i].transformedCoords[0].pos = Vector4f(intersection1);
								polygons[i].transformedCoords[1].pos = Vector4f(intersection0);
								break;
							}
							case 2:
							{
								Vector3f intersection0 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 1, 0, plane);
								Vector3f intersection1 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 1, 2, plane);

								polygons[nPolygons].transformedCoords[0].pos = Vector4f(intersection0);
								polygons[nPolygons].transformedCoords[1] = polygons[i].transformedCoords[2];
								polygons[nPolygons].transformedCoords[2] = polygons[i].transformedCoords[0];
								++nPolygons;
								++nAddedClippingPolygons;

								polygons[i].transformedCoords[0].pos = Vector4f(intersection0);
								polygons[i].transformedCoords[1].pos = Vector4f(intersection1);
								break;
							}
							case 4:
							{
								Vector3f intersection0 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 2, 0, plane);
								Vector3f intersection1 = clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 2, 1, plane);

								polygons[nPolygons].transformedCoords[0].pos = Vector4f(intersection1);
								polygons[nPolygons].transformedCoords[1] = polygons[i].transformedCoords[0];
								polygons[nPolygons].transformedCoords[2] = polygons[i].transformedCoords[1];
								++nPolygons;
								++nAddedClippingPolygons;

								polygons[i].transformedCoords[1].pos = Vector4f(intersection1);
								polygons[i].transformedCoords[2].pos = Vector4f(intersection0);
								break;
							}
							case 3:
							{
								polygons[i].transformedCoords[0].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 0, 2, plane);
								polygons[i].transformedCoords[1].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 1, 2, plane);
								break;
							}
							case 5:
							{
								polygons[i].transformedCoords[0].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 0, 1, plane);
								polygons[i].transformedCoords[2].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 2, 1, plane);
								break;
							}
							case 6:
							{
								polygons[i].transformedCoords[1].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 1, 0, plane);
								polygons[i].transformedCoords[2].pos =
									clipLineToNearPlane(polygons[i].transformedCoords, nearZ, 2, 0, plane);
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
		}
	}
	void Object::removeAddedClippingPolygons()
	{
		nPolygons -= nAddedClippingPolygons;
		nAddedClippingPolygons = 0;
	}
	void Object::doConstantShading()
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & State::ACTIVE && !(polygons[i].state & State::BACKFACE))
			{
				for (uint32_t j = 0; j < 3; ++j)
				{
					polygons[i].transformedCoords[j].shadedColor = polygons[i].transformedCoords[j].color;
				}
			}
		}
	}
	void Object::doFlatShading(const std::vector<Light>& lights)
	{
		for (uint32_t i = 0; i < nPolygons; ++i)
		{
			if (polygons[i].state & State::ACTIVE && !(polygons[i].state & State::BACKFACE))
			{
				const Color& baseColor = polygons[i].transformedCoords[0].color;
				Color result(0);
				for (auto it = lights.begin(); it != lights.end(); ++it)
				{
					if (!it->active)
						continue;

					switch (it->lightType)
					{
						case LightType::AMBIENT:
						{
							result += it->ambientIntensity * baseColor;
							break;
						}
						case LightType::DIRECTIONAL:
						{
							Vector3f u = polygons[i].transformedCoords[1].pos - polygons[i].transformedCoords[0].pos;
							Vector3f v = polygons[i].transformedCoords[2].pos - polygons[i].transformedCoords[0].pos;
							Vector3f n = u.crossProduct(v).normalize();

							float dotProduct = n.dotProduct(it->dir);
							if (dotProduct > 0.0f)
							{
								result += it->diffuseIntensity * baseColor * dotProduct;
							}
							break;
						}
						case LightType::POINT:
						{
							Vector3f u = polygons[i].transformedCoords[1].pos - polygons[i].transformedCoords[0].pos;
							Vector3f v = polygons[i].transformedCoords[2].pos - polygons[i].transformedCoords[0].pos;
							Vector3f n = u.crossProduct(v).normalize();

							Vector3f surfaceLightDir = it->pos - polygons[i].transformedCoords[0].pos;
							float dist = surfaceLightDir.getLenght();
							float attenuation = it->attenuationConstant + it->attentuationLinear * dist + it->attenuationQuadratic *
								dist * dist;
							
							float dotProduct = n.dotProduct(surfaceLightDir);
							if (dotProduct > 0)
							{
								result += (it->diffuseIntensity * baseColor * dotProduct) / attenuation;
							}
							break;
						}
					}
				}

				for (uint32_t j = 0; j < 3; ++j)
				{
					polygons[i].transformedCoords[j].shadedColor = result;
				}
			}
		}
	}
	Vector3f Object::clipLineToNearPlane(const Vertex* coords, float zNear, uint32_t p0Index, uint32_t p1Index, const Plane& plane)
	{
		assert(p0Index < 3 && p1Index < 3);

		const Vector4f* p0 = &coords[p0Index].pos;
		const Vector4f* p1 = &coords[p1Index].pos;

		Line line{ *p0, *p1 };
		auto intersectionDelta = line.getIntersectionDelta(plane);
		assert(intersectionDelta.has_value());

		Vector3f difference = *p1 - *p0;
		return (*p0) + difference * (*intersectionDelta);
	}
	Vector4f Object::computeRotation(const Vector4f& p) const
	{
		float sinX = sinf(utils::degreesToRadians(rot.x));
		float cosX = cosf(utils::degreesToRadians(rot.x));
		float sinY = sinf(utils::degreesToRadians(rot.y));
		float cosY = cosf(utils::degreesToRadians(rot.y));
		float sinZ = sinf(utils::degreesToRadians(rot.z));
		float cosZ = cosf(utils::degreesToRadians(rot.z));

		Vector4f result;
		// NOTE: The same as this but multiplied out:
		//polygons[i].transformedCoords[j].pos = Mat4x4::rotate(rot.z) * Mat4x4::rotateY(rot.y) * Mat4x4::rotate
		// (rot.x) * polygons[i].localCoords[j].pos;
		result.x = cosZ * cosY * p.x + -sinZ * cosX * p.y + sinY * p.z;
		result.y = (sinZ * cosX + cosZ * -sinX) * p.x + cosZ * cosX * p.y + cosY * sinX * p.z;
		result.z = (-sinY * cosZ + sinZ * -sinX) * p.x + (-sinZ * -sinY * cosX + cosZ * -sinX) * p.y +
			cosY * cosX * p.z;

		return result;
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
	void Material::computeReflectivities()
	{
		reflectivityA *= ambient;
		reflectivityD *= diffuse;
		reflectivityS *= specular;
	}
}

Color eg::effects::constColorPixelShader(const Vertex& vertex, const Material& material)
{
	return vertex.color;
}
