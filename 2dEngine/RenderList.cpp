#include "RenderList.h"

namespace eg
{
	void RenderList::transform(const eg::Mat4x4& transform, eg::TransformCase transformCase)
	{
		switch (transformCase)
		{
			case eg::TransformCase::LOCAL_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = vertexBuffer[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						vertexBuffer[i].localCoords[j].pos = transform * vertexBuffer[i].localCoords[j].pos;
					}
				}
				break;
			}
			case eg::TransformCase::TRANSFORM_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = vertexBuffer[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						vertexBuffer[i].transformedCoords[j].pos = transform * vertexBuffer[i].transformedCoords[j].pos;
					}
				}
				break;
			}
			case eg::TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = vertexBuffer[i].state; !(state & eg::State::ACTIVE) || (state & eg::State::BACKFACE)
						|| (state & eg::State::CLIPPED))
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						vertexBuffer[i].transformedCoords[j].pos = transform * vertexBuffer[i].localCoords[j].pos;
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

	void RenderList::translate(const Vector3f& worldPos, TransformCase transformCase)
	{
		switch (transformCase)
		{
			case TransformCase::LOCAL_COORDS_TO_TRANSFORM_COORDS:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = vertexBuffer[i].state; !(state & State::ACTIVE) || state & State::CLIPPED ||
						state & State::BACKFACE)
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						vertexBuffer[i].transformedCoords[j].pos = vertexBuffer[i].localCoords[j].pos + worldPos;
					}
				}
				break;
			}
			case TransformCase::TRANSFORM_COORDS_ONLY:
			{
				for (uint32_t i = 0; i < nPolygons; ++i)
				{
					if (uint32_t state = vertexBuffer[i].state; !(state & State::ACTIVE) || state & State::CLIPPED ||
						state & State::BACKFACE)
					{
						continue;
					}

					for (uint32_t j = 0; j < 3; ++j)
					{
						vertexBuffer[i].transformedCoords[j].pos = vertexBuffer[i].transformedCoords[j].pos + worldPos;
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
}