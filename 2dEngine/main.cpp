//Force Include GlobalDefs.h

//TODO: Eventully write your own string class (and other std stuff) yourself ;)

#include "Window.h"
#include "Graphics2d.h"
#include "Color.h"
#include "SoundSystem.h"
#include "Sound.h"
#include "Audio.h"
#include "Graphics3d.h"
#include <random>

int main()
{
	eg::Window window(900, 600, "Title", 1/60.0f);
	eg::Graphics2d& gfx = window.getGfx();
	eg::SoundSystem soundSystem = window.getSndSys();
	
	eg::Object obj;
	obj.numPolys = 1;
	
	eg::Polygon* poly = &obj.polys[0];
	for (int i = 0; i < 3; ++i)
	{
		poly->vertexList[i] = i;
	}
	poly->color = Colors::White.color;

	obj.numVertices = 3;
	obj.verticesLocal[0] = { 0.0f, 200.0f, 0.0f };
	obj.verticesLocal[1] = { 200.0f, 200.0f, 0.0f };
	obj.verticesLocal[2] = { 0.0f, 0.0f, 0.0f };
	
	eg::scaleObj(&obj, 2.0f, 2.0f, 2.0f);

	while (window.isOpen())
	{
		window.processMessages();

		gfx.clear();
		
		//transform to world space
		eg::rotateObj(&obj, 0.0f, 0.0f, 0.0f);
		obj.worldPos = { 0.0f, 0.0f, 1.0f };
		eg::transformObj(&obj);

		//transform to camera space
		for (int i = 0; i < obj.numVertices; ++i)
		{
			// ---------- vertex shader ---------
			//camera transform
			obj.verticesCamera[i] = obj.verticesWorld[i];
			//clip space
			obj.verticesClip[i].x = obj.verticesCamera[i].x * (2.0f / gfx.width);
			obj.verticesClip[i].y = obj.verticesCamera[i].y * (2.0f / gfx.height);
			obj.verticesClip[i].z = obj.verticesCamera[i].z;
			// this would also happen here: obj.verticesCamera[i].z /= farPlane; (during perspective projection matrix multiplication)

			//NOTE: One could also use these vars to do it
			const float farPlane = 100.0f;
			const float aspectRatio = ((float)gfx.width) / gfx.height;
			const float vfov = 90.0f;
			//half viewing cone angle (vfov)
			float top = tanf(utils::degreesToRadians(vfov / 2.0f));
			float bottom = -top;
			float right = top * aspectRatio;
			float left = -right;
			//NOTE: And one can compute the vFOV!
			const float fovv = eg::computeFOV(1.0f, gfx.height);
			// -----------

			// ---------- here somewhere would be the tessalation shader -----
			// ----------

			//ndc space
			eg::makeZDivide(&obj.verticesClip[i], 1.0f);
			//NOTE: to normalize z between near and far plane (0 and farPlane in this case. Not really right but yeah). Look at persProj for more information
			obj.verticesClip[i].z /= farPlane;
			//TODO: backface culling
			//TOOD: clip triangles
			obj.polys[0].clipped = false;
			//TODO: z-buffer
			obj.polys[0].visible = true;
			//viewport space
			obj.verticesViewport[i] = obj.verticesClip[i] + eg::Vector3f{ 1.0f, 1.0f, 0.0f };
			obj.verticesViewport[i] *= { gfx.width / 2.0f, gfx.height / 2.0f, 1.0f };
		}

		for (int i = 0; i < obj.numPolys; ++i)
		{
			if (obj.polys[i].visible && !obj.polys[i].clipped)
			{
				//Clip and draw each line
				for (int j = 0; j < 2; ++j)
				{
					eg::Vector3f p03f, p13f;

					p03f = obj.verticesViewport[obj.polys[i].vertexList[j]];
					p13f = obj.verticesViewport[obj.polys[i].vertexList[j + 1]];

					eg::Vector2i p0, p1;
					p0 = { (int)p03f.x, (int)p03f.y };
					p1 = { (int)p13f.x, (int)p13f.y };

					if (eg::clipLine(&p0, &p1, gfx))
					{
						// color by fragment shader
						eg::drawLine(p0, p1, obj.polys[i].color, gfx);
					}
				}

				eg::Vector3f p03f, p13f;

				p03f = obj.verticesViewport[obj.polys[i].vertexList[2]];
				p13f = obj.verticesViewport[obj.polys[i].vertexList[0]];

				eg::Vector2i p0, p1;
				p0 = { (int)p03f.x, (int)p03f.y };
				p1 = { (int)p13f.x, (int)p13f.y };

				if (eg::clipLine(&p0, &p1, gfx))
				{
					// color by fragment shader
					eg::drawLine(p0, p1, obj.polys[i].color, gfx);
				}
			}
		}

		gfx.render();

		soundSystem.update();

		window.limitFrames();
	}

	return 0;
}
