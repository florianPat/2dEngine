#pragma once

#include "Graphics2d.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace eg
{
	enum class ShadingMode
	{
		CONST_COLOR,
		FLAT,
		GOURAUD,
		PHONG,
	};

	enum class State
	{
		VISIBLE,
	};

	struct Polygon
	{
		eg::Vector3f localCoords[3];
		eg::Vector3f worldCoords[3];
		eg::Vector3f cameraCoords[3];
		eg::Vector3f clipCoords[3];
		eg::Vector2f screenCoords[3];
		Color color;
		ShadingMode shadingMode;
		State state = State::VISIBLE;
	};

	struct Object
	{
		eg::Vector3f pos;
		eg::Quaternion rot;
		eg::Vector3f scl;
		uint32_t nPolygons;

		Polygon polygon[265];
	};
}