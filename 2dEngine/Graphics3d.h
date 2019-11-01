#pragma once

#include "Vector3.h"
#include "Mat4x4.h"
#include <string>
#include "Vector2.h"
#include "Color.h"
#include "Graphics2d.h"

namespace eg
{

	struct Dir3d
	{
		float angX, angY, angZ;
	};

	struct Polygon
	{
		int32_t vertexList[3]; // Index buffer, index number of vertices
		int32_t color; //color of the polygon
		int32_t shade; //the final shade of color after lighting
		int32_t shading; //type of lighting, flat or constant shading
		int32_t twoSided; //flags if the polygon is two sided
		int32_t visible; //used to remove backfaces
		int32_t active; //used to turn faces on an off
		int32_t clipped; //flags that polygon has been clipped or removed
		float normalLength; //pre-computed magnitude of normal
	};

	struct Facet
	{
		int32_t numPoints; //number of vertices
		int32_t color; //color of polygon
		int32_t shade; //the final shade of the color after lighting
		int32_t shading; //type of shading to use
		int32_t twoSided; //is the facet two sided
		int32_t visible; //is the facet transparent
		int32_t clipped; //has this poly been clipped
		int32_t active; //used to turn faces on and off
		Vector3f vertexList[3]; //the points that make up the polygon facet, vertex buffer
	};

	struct Object
	{
		static constexpr int32_t MAX_VERTICES_PER_OBJECT = 32;
		static constexpr int32_t MAX_POLYS_PER_OBJECT = 16;
		int32_t id;
		int32_t numVertices;
		Vector3f verticesLocal[MAX_VERTICES_PER_OBJECT]; //relative to own local origin
		Vector3f verticesWorld[MAX_VERTICES_PER_OBJECT]; //result of transformation to world coordinates
		Vector3f verticesCamera[MAX_VERTICES_PER_OBJECT]; //result of transformation to camera space
		Vector3f verticesClip[MAX_VERTICES_PER_OBJECT];
		Vector3f verticesViewport[MAX_VERTICES_PER_OBJECT];
		int32_t numPolys;
		Polygon polys[MAX_POLYS_PER_OBJECT];
		float radius; //average radius of the object
		Vector3f worldPos;
	};

	Object objLoadObject(const std::string& filename, float scaleFactor = 1.0f);
	void drawLine(Vector2i p0, Vector2i p1, Color color, Graphics2d& gfx);
	bool clipLine(Vector2i* p0, Vector2i* p1, const Graphics2d& gfx);
	float computeFOV(float nearPlane, int screenDimension);
	void makeZDivide(Vector3f* p, float nearPlane);
	void rotateObj(Object* obj, float rotX, float rotY, float rotZ);
	void scaleObj(Object* obj, float sclX, float sclY, float sclZ);
	void transformObj(Object* obj);
}