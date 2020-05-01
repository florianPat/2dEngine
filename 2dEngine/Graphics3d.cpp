#include "Graphics3d.h"

namespace eg
{
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
}