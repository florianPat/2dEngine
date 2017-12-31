#include "Graphics2d.h"

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

		if (sprite.pos.x < 0)
			clipX = (int)-sprite.pos.x;
		else if (sprite.pos.x + sprite.texture.getWidth() >= width)
			clipWidth = (int)sprite.pos.x + sprite.texture.getWidth() - width + 1;

		if (sprite.pos.y < 0)
			clipY = (int)-sprite.pos.y;
		else if (sprite.pos.y + sprite.texture.getHeight() >= height)
			clipHeight = (int)sprite.pos.y + sprite.texture.getHeight() - height + 1;

		for (int y = clipY; y < sprite.texture.getHeight() - clipHeight; ++y)
		{
			for (int x = clipX; x < sprite.texture.getWidth() - clipWidth; ++x)
			{
				Color c;

				Color sc = sprite.texture.getPixel(x, y);
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

	void Graphics2d::render()
	{
		HDC dc = GetDC(windowHandle);

		StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, (void*)backBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(windowHandle, dc);
	}
}
