#pragma once

#include "Graphics3d.h"

class RenderList
{
	uint32_t nPolygons;
	eg::Polygon* indexBuffer[64];
	eg::Polygon vertexBuffer[128];
};