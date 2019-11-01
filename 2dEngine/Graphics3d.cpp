#include "Graphics3d.h"
#include <fstream>
#include <math.h>

namespace eg
{
	Object objLoadObject(const std::string & filename, float scaleFactor)
	{
		Object result = { 0 };

		std::ifstream file(filename);
		assert(file);

		std::string line;
		int vertexListCount = 0;
		int indexListCount = 0;
		while (!file.eof())
		{
			std::getline(file, line);

			switch (line[0])
			{
				case 'v':
				{
					size_t offset = 2;
					Vector3f vertex;
					float* vertexP = (float*)&vertex;

					for (int i = 0; i < 3; ++i)
					{
						size_t spacePos = line.find(' ', offset);
						std::string vertexCoord = line.substr(offset, spacePos - offset);
						vertexP[i] = (float)atof(vertexCoord.c_str());
					}

					result.verticesLocal[vertexListCount++] = vertex;
					break;
				}
				case 'f':
				{
					Polygon* poly = &result.polys[indexListCount++];
					poly->active = true;
					poly->clipped = false;
					poly->color = Colors::White.color;
					poly->normalLength = 1.0f;
					poly->shading = 0;
					poly->twoSided = false;
					poly->visible = true;

					size_t offset = 2;
					for (int i = 0; i < 3; ++i)
					{
						size_t spacePos = line.find(' ', offset);
						std::string vertexCoord = line.substr(offset, spacePos - offset);
						poly->vertexList[i] = (atoi(vertexCoord.c_str())) - 1;
					}
					break;
				}
			}
		}

		result.numPolys = indexListCount;
		result.numVertices = vertexListCount;
		result.worldPos = { 0.0f, 0.0f, 0.0f };
		result.radius = 0.0f;

		Vector3f radius = { 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < result.numVertices; ++i)
		{
			if (result.verticesLocal[i].x > radius.x)
			{
				radius.x = result.verticesLocal[i].x;
			}
			if (result.verticesLocal[i].y > radius.y)
			{
				radius.y = result.verticesLocal[i].y;
			}
			if (result.verticesLocal[i].z > radius.z)
			{
				radius.z = result.verticesLocal[i].z;
			}
		}
		result.radius = radius.x > radius.y ? radius.x : radius.y;
		result.radius = radius.z > result.radius ? radius.z : result.radius;

		return result;
	}

	void drawLine(Vector2i p0, Vector2i p1, Color color, Graphics2d& gfx)
	{
		int32_t dx, dy, xInc, yInc, error = 0, x = p0.x, y = p0.y;

		//compute horizontal and vertical deltas
		dx = p1.x - p0.x;
		dy = p1.y - p0.y;

		//test which direction the line is going in i.e. slope angle
		if (dx > 0)
		{
			xInc = 1;
		}
		else
		{
			xInc = -1;
			dx = -dx; //need absolute value
		}

		if (dy > 0)
		{
			yInc = 1;
		}
		else
		{
			yInc = -1;
			dy = -dy; //need absolute value of dy
		}

		//based on which delta is greater, we can draw the line
		if (dx > dy)
		{
			for (int32_t i = 0; i <= dx; ++i)
			{
				gfx.putPixel(x, y, color);

				//One understands it if one thinks about it graphicly!
				//If dx = 2 and dy = 1, we need to go two in x, before we increment y, so that this gets created:
				// xx
				//   xx ...
				//Because we go 2 in x (dx), 1 in y(dy); Thats the "pattern"
				error += dy;

				if (error > dx)
				{
					error -= dx;
					y += yInc;
				}

				x += xInc;
			}
		}
		else
		{
			for (int32_t i = 0; i <= dy; ++i)
			{
				gfx.putPixel(x, y, color);

				error += dx;

				if (error > 0)
				{
					error -= dy;

					x += xInc;
				}

				y += yInc;
			}
		}
	}

	bool clipLine(Vector2i * p0, Vector2i * p1, const Graphics2d & gfx)
	{
		float dy = ((float)p1->y - p0->y) / (p1->x - p0->x);
		float dx = ((float)p1->x - p0->x) / (p1->y - p0->y);
		//b = y - mx, because y = mx + b
		float by = (float)p0->y - dy * p0->x;
		float bx = (float)p0->x - dx * p0->y;

		if (p0->x < 0)
		{
			p0->x = 0;
			p0->y = (int)by;
		}
		else if (p0->x >= gfx.width)
		{
			p0->x = gfx.width - 1;
			p0->y = (int)(dy * (gfx.width - 1) + by);
		}
		if (p0->y < 0)
		{
			p0->y = 0;
			p0->x = (int)bx;
		}
		else if (p0->y >= gfx.height)
		{
			p0->y = gfx.height - 1;
			p0->x = (int)(dx * (gfx.height - 1) + bx);
		}

		if (p1->x < 0)
		{
			p1->x = 0;
			p1->y = (int)by;
		}
		else if (p1->x >= gfx.width)
		{
			p1->x = gfx.width - 1;
			p1->y = (int)(dy * (gfx.width - 1) + by);
		}
		if (p1->y < 0)
		{
			p1->y = 0;
			p1->x = (int)bx;
		}
		else if (p1->y >= gfx.height)
		{
			p1->y = gfx.height - 1;
			p1->x = (int)(dx * (gfx.height - 1) + bx);
		}

		if (*p0 == *p1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

#if 0
	int32_t clipLine(Vector2i & p0, Vector2i & p1, const Graphics2d& gfx)
	{
		int32_t poly_clip_min_x = 0,
				poly_clip_min_y = 0,
				poly_clip_max_x = gfx.width,
				poly_clip_max_y = gfx.height;
		// this function clips the sent line using the globally defined clipping
		// region
		int point_1 = 0, point_2 = 0; // tracks if each end point is visible or invisible
			
		int clip_always = 0;
		// used for clipping override
		int xi, yi;
		// point of intersection
		int right_edge = 0,
			// which edges are the endpoints beyond
			left_edge = 0,
			top_edge = 0,
			bottom_edge = 0;
		int success = 0;
		// was there a successful clipping

		// test if line is completely visible
		if ((*x1 >= poly_clip_min_x) && (*x1 <= poly_clip_max_x) &&
			(*y1 >= poly_clip_min_y) && (*y1 <= poly_clip_max_y))
			point_1 = 1;
		if ((*x2 >= poly_clip_min_x) && (*x2 <= poly_clip_max_x) &&
			(*y2 >= poly_clip_min_y) && (*y2 <= poly_clip_max_y))
			point_2 = 1;
		// test endpoints
		if (point_1 == 1 && point_2 == 1)
			return(1);
		// test if line is completely invisible
		if (point_1 == 0 && point_2 == 0)
		{
			// must test to see if each endpoint is on the same side of one of
			// the bounding planes created by each clipping region boundary
			if (((*x1 < poly_clip_min_x) && (*x2 < poly_clip_min_x)) || // to the left
				((*x1 > poly_clip_max_x) && (*x2 > poly_clip_max_x)) || // to the right
				((*y1 < poly_clip_min_y) && (*y2 < poly_clip_min_y)) || // above
				((*y1 > poly_clip_max_y) && (*y2 > poly_clip_max_y)))   // below
			{
				// no need to draw line
				return(0);
			} // end if invisible
			// if we got here we have the special case where the line cuts into and
			// out of the clipping region
			clip_always = 1;
		} // end if test for invisibility
		// take care of case where either endpoint is in clipping region
		if ((point_1 == 1) || (point_1 == 0 && point_2 == 0))
		{
			// compute deltas
			dx = *x2 - *x1;
			dy = *y2 - *y1;
			// compute what boundary lines need to be clipped against
			if (*x2 > poly_clip_max_x)
			{
				// flag right edge
				right_edge = 1;
				// compute intersection with right edge
				if (dx != 0)
				yi = (int)(.5 + (dy / dx) * (poly_clip_max_x - *x1) + *y1);
				else
				yi = -1;
				// invalidate intersection
			}
			else if (*x2 < poly_clip_min_x)
			{
				// flag left edge
				left_edge = 1;
				// compute intersection with left edge
				if (dx != 0)
					yi = (int)(.5 + (dy / dx) * (poly_clip_min_x - *x1) + *y1);
				else
					yi = -1;
				// invalidate intersection
			}
			// horizontal intersections
			if (*y2 > poly_clip_max_y)
			{
				// flag bottom edge
				bottom_edge = 1;
				// compute intersection with right edge
				if (dy != 0)
					xi = (int)(.5 + (dx / dy) * (poly_clip_max_y - *y1) + *x1);
				else
					xi = -1;
				// invalidate intersection
			} // end if bottom
			else if (*y2 < poly_clip_min_y)
			{
				// flag top edge
				top_edge = 1;
				// compute intersection with top edge
				if (dy != 0)
					xi = (int)(.5 + (dx / dy) * (poly_clip_min_y - *y1) + *x1);
				else
					xi = -1;
			// invalidate intersection
			} // end if top
			// now we know where the line passed thru
			// compute which edge is the proper intersection
			if (right_edge == 1 && (yi >= poly_clip_min_y && yi <= poly_clip_max_y))
			{
				*x2 = poly_clip_max_x;
				*y2 = yi;
				success = 1;
			} // end if intersected right edge
			else if (left_edge == 1 && (yi >= poly_clip_min_y && yi <= poly_clip_max_y))
			{
				*x2 = poly_clip_min_x;
				*y2 = yi;
				success = 1;
			} // end if intersected left edge
			if (bottom_edge == 1 && (xi >= poly_clip_min_x && xi <= poly_clip_max_x))
			{
				*x2 = xi;
				*y2 = poly_clip_max_y;
				success = 1;
			} // end if intersected bottom edge
			else if (top_edge == 1 && (xi >= poly_clip_min_x && xi <= poly_clip_max_x))
			{
				*x2 = xi;
				*y2 = poly_clip_min_y;
				success = 1;
			} // end if intersected top edge
		} // end if point_1 is visible
		// reset edge flags
		right_edge = left_edge = top_edge = bottom_edge = 0;
		// test second endpoint
		if ((point_2 == 1) || (point_1 == 0 && point_2 == 0))
		{
			// compute deltas
			dx = *x1 - *x2;
			dy = *y1 - *y2;
			// compute what boundary line needs to be clipped against
			if (*x1 > poly_clip_max_x)
			{
				// flag right edge
				right_edge = 1;
				// compute intersection with right edge
				if (dx != 0)
					yi = (int)(.5 + (dy / dx) * (poly_clip_max_x - *x2) + *y2);
				else
					yi = -1;
				// invalidate intersection
			} // end if to right
			else if (*x1 < poly_clip_min_x)
			{
				// flag left edge
				left_edge = 1;
				// compute intersection with left edge
				if (dx != 0)
					yi = (int)(.5 + (dy / dx) * (poly_clip_min_x - *x2) + *y2);
				else
					yi = -1;
				// invalidate intersection
			} // end if to left
			// horizontal intersections
			if (*y1 > poly_clip_max_y)
			{
				// flag bottom edge
				bottom_edge = 1;
				// compute intersection with right edge
				if (dy != 0)
					xi = (int)(.5 + (dx / dy) * (poly_clip_max_y - *y2) + *x2);
				else
					xi = -1;
				// invalidate inntersection
			} // end if bottom
			else if (*y1 < poly_clip_min_y)
			{
				// flag top edge
				top_edge = 1;
				// compute intersection with top edge
				if (dy != 0)
					xi = (int)(.5 + (dx / dy) * (poly_clip_min_y - *y2) + *x2);
				else
					xi = -1;
				// invalidate inntersection
			} // end if top
			// now we know where the line passed thru
			// compute which edge is the proper intersection
			if (right_edge == 1 && (yi >= poly_clip_min_y && yi <= poly_clip_max_y))
			{
				*x1 = poly_clip_max_x;
				*y1 = yi;
				success = 1;
			} // end if intersected right edge
			else if (left_edge == 1 && (yi >= poly_clip_min_y && yi <= poly_clip_max_y))
			{
				*x1 = poly_clip_min_x;
				*y1 = yi;
				success = 1;
			} // end if intersected left edge
			if (bottom_edge == 1 && (xi >= poly_clip_min_x && xi <= poly_clip_max_x))
			{
				*x1 = xi;
				*y1 = poly_clip_max_y;
				success = 1;
			} // end if intersected bottom edge
			else if (top_edge == 1 && (xi >= poly_clip_min_x && xi <= poly_clip_max_x))
			{
				*x1 = xi;
				*y1 = poly_clip_min_y;
				success = 1;
			} // end if intersected top edge
		} // end if point_2 is visible
		return success;
	}
#endif

	float computeFOV(float nearPlane, int screenDimension)
	{
		// FOV = 2 * Winkel zwischen viewingDistance und screenWidth / -height
		// tan wegen An- und Gegenkathete
		//				 - gegenkatete -----   -- adjecient --
		return 2 * atanf(screenDimension / 2 * nearPlane);
	}

	void makeZDivide(Vector3f * p, float nearPlane)
	{
		//NOTE: times viewingDistance because point we could project onto could not be one away thing;)
		p->x = nearPlane * p->x / p->z;
		p->y = nearPlane * p->y / p->z;
	}

	void rotateObj(Object * obj, float rotX, float rotY, float rotZ)
	{
		Mat4x4 rot = Mat4x4::rotateX(rotX);
		rot *= Mat4x4::rotateY(rotY);
		rot *= Mat4x4::rotateZ(rotZ);

		for (int32_t i = 0; i < obj->numVertices; ++i)
		{
			obj->verticesLocal[i] = rot * obj->verticesLocal[i];
		}
	}
	void scaleObj(Object * obj, float sclX, float sclY, float sclZ)
	{
		Mat4x4 scl = Mat4x4::scale({ sclX, sclY, sclZ });

		for (int32_t i = 0; i < obj->numVertices; ++i)
		{
			obj->verticesLocal[i] = scl * obj->verticesLocal[i];
		}
	}
	void transformObj(Object * obj)
	{
		Mat4x4 transform = Mat4x4::translate(obj->worldPos);

		for (int32_t i = 0; i < obj->numVertices; ++i)
		{
			obj->verticesWorld[i] = transform * obj->verticesLocal[i];
		}
	}
}