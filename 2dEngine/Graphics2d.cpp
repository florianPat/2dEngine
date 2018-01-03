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
		Vei2 circleCenter((int)circle.center.x, (int)circle.center.y);
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
				Vei2 point(x, y);
				Vei2 lengthVec(circleCenter - point);
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

		Vec2f xAxis = { cosf(sprite.rotation), sinf(sprite.rotation) };
		xAxis.normalize();
		xAxis *= (float)sprite.texture.getWidth();
		Vec2f yAxis = { (-sinf(sprite.rotation)), cosf(sprite.rotation) };
		yAxis.normalize();
		yAxis *= (float)sprite.texture.getHeight();

		Vec2f points[4] = { {sprite.pos}, {sprite.pos.x + sprite.texture.getWidth(), sprite.pos.y}, 
							{sprite.pos.x, sprite.pos.y + sprite.texture.getHeight()}, {sprite.pos.x + sprite.texture.getWidth(), sprite.pos.y + sprite.texture.getHeight()} };

		Vec2f origin = sprite.pos + sprite.origin;

		for (int i = 0; i < 4; ++i)
		{
			points[i] = Vec2f(sprite.pos + xAxis * (points[i].x - origin.x) + yAxis * (points[i].y - origin.y));
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
				Vec2f testVec((float)x, (float)y);

				float edge1 = -yAxis * (testVec - sprite.pos);
				float edge2 = xAxis * (testVec - (sprite.pos + xAxis));
				float edge3 = yAxis * (testVec - (sprite.pos + xAxis + yAxis));
				float edge4 = -xAxis * (testVec - (sprite.pos + yAxis));

				if (edge1 <= 0 && edge2 <= 0 &&
					edge3 <= 0 && edge4 <= 0)
				{
					Vec2f point = testVec - sprite.pos;
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
		Vec2f points[4];
		Vec2f axis[2];
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
				Vec2f testVec((float)x, (float)y);

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

	void Graphics2d::render()
	{
		HDC dc = GetDC(windowHandle);

		StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, (void*)backBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(windowHandle, dc);
	}
}
