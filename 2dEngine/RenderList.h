#pragma once

#include "Graphics3d.h"

namespace eg
{
	struct RenderList
	{
		uint32_t nPolygons;
		uint32_t nIndicies;
		Polygon* indexBuffer[64];
		Polygon vertexBuffer[128];

		void transform(const Mat4x4& transform, TransformCase transformCase);
		void translate(const Vector3f& worldPos, TransformCase transformCase = eg::TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS);
	};
}