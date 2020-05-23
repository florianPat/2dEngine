#include "Graphics2d.h"

namespace eg
{
	float Lerp(float a, float t, float b)
	{
		return ((1.0f - t)*a + t * b);
	}
}

namespace eg
{
	Graphics2d::Graphics2d(HWND& windowHandle, int width, int height)
		: windowHandle(windowHandle), width(width),
		height(height), pitch(width),
		backBuffer(new uint32_t[width * height])
	{
		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = width;
		bitmapInfo.bmiHeader.biHeight = -height;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
	}

	void Graphics2d::ClipRect(int & x, int & y)
	{
		if (x < 0)
			x = 0;
		else if (x >= (int)width)
			x = width - 1;

		if (y < 0)
			y = 0;
		else if (y >= (int)height)
			y = height - 1;
	}

	void Graphics2d::drawFlatBottomTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, Color color)
	{
		Vector2i clipP0, clipP1;

		float dyLeft = ((float)p1.x - p0.x) / (p1.y - p0.y);
		float dyRight = ((float)p2.x - p0.x) / (p2.y - p0.y);

		if(p0.x >= 0 && p0.x < (int32_t)width && p0.y >= 0 && p0.y < (int32_t)height)
			putPixel(p0.x, p0.y, color);

		float x0 = (float)p0.x, x1 = (float)p0.x;
		for (int i = p0.y + 1; i < p1.y; ++i)
		{
			x0 += dyLeft;
			x1 += dyRight;

			clipP0 = { (int32_t)x0, i };
			clipP1 = { (int32_t)x1, i };
			if (clipLine(clipP0, clipP1))
				drawLine(clipP0, clipP1, color);
		}

		clipP0 = p0;
		clipP1 = p1;
		if (clipLine(clipP0, clipP1))
			drawLine(clipP0, clipP1, color);
	}

	void Graphics2d::drawFlatTopTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, Color color)
	{
		Vector2i clipP0, clipP1;

		float dyLeft = ((float)p2.x - p0.x) / (p2.y - p0.y);
		float dyRight = ((float)p2.x - p1.x) / (p2.y - p1.y);

		clipP0 = p0;
		clipP1 = p1;
		if (clipLine(clipP0, clipP1))
			drawLine(clipP0, clipP1, color);

		float x0 = (float)p0.x, x1 = (float)p1.x;
		for (int32_t i = p0.y + 1; i < p2.y; ++i)
		{
			x0 += dyLeft;
			x1 += dyRight;

			clipP0 = { (int32_t)x0, i };
			clipP1 = { (int32_t)x1, i };

			if (clipLine(clipP0, clipP1))
				drawLine(clipP0, clipP1, color);
		}

		if (p2.x >= 0 && p2.x < (int32_t)width && p2.y >= 0 && p2.y < (int32_t)height)
			putPixel(p2.x, p2.y, color);
	}

	Graphics2d::~Graphics2d()
	{
		delete [] backBuffer;
		backBuffer = nullptr;
	}

	void Graphics2d::clear()
	{
		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				backBuffer[x + y * pitch] = 0;
			}
		}
	}

	void Graphics2d::putPixel(int x, int y, Color c)
	{
		backBuffer[x + y * pitch] = ((c.GetR()) | ((c.GetG()) << 8) | ((c.GetB()) << 16) | (c.GetA() << 24));
	}

	Color Graphics2d::getPixel(uint x, uint y)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		return backBuffer[x + y * width];
	}

	void Graphics2d::draw(RectI& rect)
	{
		int x0 = rect.left;
		int x1 = rect.right;
		int y0 = rect.top;
		int y1 = rect.bottom;

		ClipRect(x0, y0);
		ClipRect(x1, y1);

		assert(x0 >= 0 && x0 <= (int)width);
		assert(y0 >= 0 && y0 <= (int)height);
		assert(x1 >= 0 && x1 <= (int)width);
		assert(y1 >= 0 && y1 <= (int)height);

		if (x0 > x1)
		{
			int temp = x0;
			x0 = x1;
			x1 = temp;
		}
		if (y0 > y1)
		{
			int temp = y0;
			y0 = y1;
			y1 = temp;
		}

		for (int y = y0; y < y1; ++y)
		{
			for (int x = x0; x < x1; ++x)
			{
				putPixel(x, y, rect.color);
			}
		}
	}

	void Graphics2d::draw(RectF & rect)
	{
		RectI drawRect((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom, rect.color);
		draw(drawRect);
	}

	void Graphics2d::draw(FloatCircle & circle)
	{
		int diameter = 2 * (int)circle.radius;
		Vector2i circleCenter((int)circle.center.x, (int)circle.center.y);
		int radiusSq = (int)circle.radius * (int)circle.radius;
		RectI boundingRect({ (int)circle.center.x - (int)circle.radius, (int)circle.center.y - (int)circle.radius }, diameter, diameter);

		ClipRect(boundingRect.left, boundingRect.top);
		ClipRect(boundingRect.right, boundingRect.bottom);

		assert(boundingRect.left >= 0 && boundingRect.left <= (int)width);
		assert(boundingRect.top >= 0 && boundingRect.top <= (int)height);
		assert(boundingRect.right >= 0 && boundingRect.right <= (int)width);
		assert(boundingRect.bottom >= 0 && boundingRect.bottom <= (int)height);

		for (int y = boundingRect.top; y <= boundingRect.bottom; ++y)
		{
			for (int x = boundingRect.left; x <= boundingRect.right; ++x)
			{
				Vector2i point(x, y);
				Vector2i lengthVec(circleCenter - point);
				if (lengthVec.getLenghtSq() <= radiusSq)
				{
					putPixel(point.x, point.y, circle.color);
				}
			}
		}
	}

	void Graphics2d::draw(const Sprite & sprite)
	{
		int clipX = 0, clipY = 0, clipWidth = 0, clipHeight = 0;

		Vector2f xAxis = { cosf(sprite.rotation), sinf(sprite.rotation) };
		xAxis.normalize();
		xAxis *= (float)sprite.texture.getWidth() * sprite.scale;
		Vector2f yAxis = { (-sinf(sprite.rotation)), cosf(sprite.rotation) };
		yAxis.normalize();
		yAxis *= (float)sprite.texture.getHeight() * sprite.scale;

		Vector2f points[4] = { {sprite.pos}, {sprite.pos.x + sprite.texture.getWidth(), sprite.pos.y}, 
							{sprite.pos.x, sprite.pos.y + sprite.texture.getHeight()}, {sprite.pos.x + sprite.texture.getWidth(), sprite.pos.y + sprite.texture.getHeight()} };

		Vector2f origin = sprite.pos + sprite.origin;

		for (int i = 0; i < 4; ++i)
		{
			points[i] = Vector2f(sprite.pos + xAxis * (points[i].x - origin.x) + yAxis * (points[i].y - origin.y));
		}

		RectF boundingRect(min(min(points[0].x, points[1].x), min(points[2].x, points[3].x)),
			min(min(points[0].y, points[1].y), min(points[2].y, points[3].y)),
			max(max(points[0].x, points[1].x), max(points[2].x, points[3].x)),
			max(max(points[0].y, points[1].y), max(points[2].y, points[3].y)));

		if (boundingRect.left < 0.0f)
			boundingRect.left = 0;
		if (boundingRect.right >= (float)width)
			boundingRect.right = width - 1.0f;
		if (boundingRect.top < 0.0f)
			boundingRect.top = 0.0f;
		if (boundingRect.bottom >= (float)height)
			boundingRect.bottom = height - 1.0f;

		for (int y = (int)boundingRect.top; y < (int)boundingRect.bottom; ++y)
		{
			for (int x = (int)boundingRect.left; x < (int)boundingRect.right; ++x)
			{
				Vector2f testVec((float)x, (float)y);

				float edge1 = -yAxis * (testVec - sprite.pos);
				float edge2 = xAxis * (testVec - (sprite.pos + xAxis));
				float edge3 = yAxis * (testVec - (sprite.pos + xAxis + yAxis));
				float edge4 = -xAxis * (testVec - (sprite.pos + yAxis));

				if (edge1 <= 0 && edge2 <= 0 &&
					edge3 <= 0 && edge4 <= 0)
				{
					Vector2f point = testVec - sprite.pos;
					float u = (point * xAxis) / xAxis.getLenghtSq();
					float v = (point * yAxis) / yAxis.getLenghtSq();

					float texelX = u * (sprite.texture.getWidth() - 2);
					float texelY = v * (sprite.texture.getHeight() - 2);

					int iX = (int)texelX;
					int iY = (int)texelY;

					float fX = texelX - iX;
					float fY = texelY - iY;

					Color texelA = sprite.texture.getPixel(iX, iY), texelB = sprite.texture.getPixel(iX + 1, iY), texelC = sprite.texture.getPixel(iX, iY + 1),
						texelD = sprite.texture.getPixel(iX + 1, iY + 1);

					Color c;

					Color sc;
					sc.SetR((uchar)(Lerp(Lerp(texelA.GetR() / 255.0f, fX, texelB.GetR() / 255.0f), fY, Lerp(texelC.GetR() / 255.0f, fX, texelD.GetR() / 255.0f)) * 255));
					sc.SetG((uchar)(Lerp(Lerp(texelA.GetG() / 255.0f, fX, texelB.GetG() / 255.0f), fY, Lerp(texelC.GetG() / 255.0f, fX, texelD.GetG() / 255.0f)) * 255));
					sc.SetB((uchar)(Lerp(Lerp(texelA.GetB() / 255.0f, fX, texelB.GetB() / 255.0f), fY, Lerp(texelC.GetB() / 255.0f, fX, texelD.GetB() / 255.0f)) * 255));
					sc.SetA((uchar)(Lerp(Lerp(texelA.GetA() / 255.0f, fX, texelB.GetA() / 255.0f), fY, Lerp(texelC.GetA() / 255.0f, fX, texelD.GetA() / 255.0f)) * 255));

					float rScA = sc.GetA() / 255.0f;
					
					Color dc = getPixel(x + (int)sprite.pos.x, y + (int)sprite.pos.y);
					float rDcA = dc.GetA() / 255.0f;

					c.SetR((char)((sc.GetR() + (dc.GetR() * (1 - rScA))) + 0.5f));
					c.SetG((char)((sc.GetG() + (dc.GetG() * (1 - rScA))) + 0.5f));
					c.SetB((char)((sc.GetB() + (dc.GetB() * (1 - rScA))) + 0.5f));
					c.SetA((char)((rScA + rDcA - rScA * rDcA) * 255.0f));

					putPixel(x + (uint)sprite.pos.x, y + (uint)sprite.pos.y, c);
				}
			}
		}
	}

	void Graphics2d::draw(const ORectF & oRect)
	{
		Vector2f points[4];
		Vector2f axis[2];
		oRect.getPointsAxis(points, axis);

		RectF boundingRect(min(min(points[0].x, points[1].x), min(points[2].x, points[3].x)), 
						   min(min(points[0].y, points[1].y), min(points[2].y, points[3].y)), 
			               max(max(points[0].x, points[1].x), max(points[2].x, points[3].x)), 
			               max(max(points[0].y, points[1].y), max(points[2].y, points[3].y)));

		if (boundingRect.left < 0)
			boundingRect.left = 0;
		if (boundingRect.right >= width)
			boundingRect.right = width - 1.0f;
		if (boundingRect.top < 0)
			boundingRect.top = 0;
		if (boundingRect.bottom >= height)
			boundingRect.bottom = height - 1.0f;

		for (int y = (int)boundingRect.top; y < (int)boundingRect.bottom; ++y)
		{
			for (int x = (int)boundingRect.left; x < (int)boundingRect.right; ++x)
			{
				Vector2f testVec((float)x, (float)y);

				float edge1 = -axis[1] * (testVec - oRect.pos);
				float edge2 = axis[0] * (testVec - (oRect.pos + (oRect.xAxis * oRect.width)));
				float edge3 = axis[1] * (testVec - (oRect.pos + (oRect.xAxis * oRect.width) + (oRect.yAxis * oRect.height)));
				float edge4 = -axis[0] * (testVec - (oRect.pos + (oRect.yAxis * oRect.height)));

				if (edge1 <= 0 && edge2 <= 0 &&
					edge3 <= 0 && edge4 <= 0)
					putPixel(x, y, oRect.color);
			}
		}
	}

	void Graphics2d::drawLine(const Vector2i& p0, const Vector2i& p1, Color color)
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
				putPixel(x, y, color);

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
				putPixel(x, y, color);

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

	bool Graphics2d::clipLine(Vector2i& p0, Vector2i& p1)
	{
		Vector2f p0f = (Vector2f)p0;
		Vector2f p1f = (Vector2f)p1;

		float m = (p1f.y - p0f.y) / (p1f.x - p0f.x);

		if (p0f.x < 0.0f)
		{
			p0f.y = m * (-p0f.x) + p0f.y;
			if (p0f.y < 0.0f || p0f.y >= height)
				return false;
			p0f.x = 0.0f;
		}
		else if (p0f.x >= width)
		{
			p0f.y = m * ((width - 1) - p0f.x) + p0f.y;
			if (p0f.y < 0.0f || p0f.y >= height)
				return false;
			p0f.x = width - 1.0f;
		}

		if (p1f.x < 0.0f)
		{
			p1f.y = m * (-p0f.x) + p0f.y;
			p1f.x = 0.0f;
		}
		else if (p1f.x >= width)
		{
			p1f.y = m * ((width - 1) - p0f.x) + p0f.y;
			p1f.x = width - 1.0f;
		}

		if (p0f.y < 0.0f)
		{
			p0f.x = (-p0f.y) / m + p0f.x;
			if (p0f.x < 0.0f || p0f.x >= width)
				return false;
			p0f.y = 0.0f;
		}
		else if (p0f.y >= height)
		{
			p0f.x = ((height - 1) - p0f.y) / m + p0f.x;
			if (p0f.x < 0.0f || p0f.x >= width)
				return false;
			p0f.y = height - 1.0f;
		}

		if (p1f.y < 0.0f)
		{
			p1f.x = (-p0f.y) / m + p0f.x;
			if (p1f.x < 0.0f || p1f.x >= width)
				return false;
			p1f.y = 0.0f;
		}
		else if (p1f.y >= height)
		{
			p1f.x = ((height - 1) - p0f.y) / m + p0f.x;
			if (p1f.x < 0.0f || p1f.x >= width)
				return false;
			p1f.y = height - 1.0f;
		}

		p0 = (Vector2i)p0f;
		p1 = (Vector2i)p1f;

		return true;
	}

	void Graphics2d::drawPolyline(std::vector<Vector2i> points, Color color)
	{
		for(uint32_t i = 0; i < points.size() - 1; ++i)
		{
			if (clipLine(points[i], points[i + 1]))
				drawLine(points[i], points[i + 1], color);
		}

		if(clipLine(points[points.size() - 1], points[0]))
			drawLine(points[points.size() - 1], points[0], color);
	}

	void Graphics2d::drawTriangle(Vector2i p0, Vector2i p1, Vector2i p2, Color color)
	{
		Vector2i temp;

		if (p0.y > p2.y)
		{
			temp = p2;
			p2 = p0;
			p0 = temp;
		}
		if (p0.y > p1.y)
		{
			temp = p1;
			p1 = p0;
			p0 = temp;
		}
		if (p1.y > p2.y)
		{
			temp = p2;
			p2 = p1;
			p1 = temp;
		}

		if (p0.y == p1.y)
		{
			if (p0.x > p1.x)
			{
				temp = p1;
				p1 = p0;
				p0 = temp;
			}

			drawFlatTopTriangle(p0, p1, p2, color);
		}
		else if (p1.y == p2.y)
		{
			if (p1.x > p2.x)
			{
				temp = p2;
				p2 = p1;
				p1 = temp;
			}

			drawFlatBottomTriangle(p0, p1, p2, color);
		}
		else
		{
			if (p1.x > p0.x)
			{
				float dyLeft = ((float)p2.x - p0.x) / (p2.y - p0.y);
				
				Vector2i realP2 = { (int32_t)(dyLeft * (p1.y - p0.y) + p0.x), p1.y };
				drawFlatBottomTriangle(p0, realP2, p1, color);
				drawFlatTopTriangle(realP2, p1, p2, color);
			}
			else
			{
				float dyRight = ((float)p2.x - p0.x) / (p2.y - p0.y);

				Vector2i realP2 = { (int32_t)(dyRight * (p1.y - p0.y) + p0.x), p1.y };
				drawFlatBottomTriangle(p0, p1, realP2, color);
				drawFlatTopTriangle(p1, realP2, p2, color);
			}
		}
	}

	void Graphics2d::render()
	{
		HDC dc = GetDC(windowHandle);

		StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, (void*)backBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(windowHandle, dc);
	}
}
