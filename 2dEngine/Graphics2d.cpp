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

	void Graphics2d::putPixel(int x, int y, int r, int g, int b)
	{
		backBuffer[x + y * pitch] = (((uint8_t)r) | (((uint8_t)g) << 8) | (((uint8_t)b) << 16));
	}

	void Graphics2d::drawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, int r, int g, int b)
	{
		assert(x0 >= 0 && x0 <= width);
		assert(y0 >= 0 && y0 <= height);
		assert(x1 >= 0 && x1 <= width);
		assert(y1 >= 0 && y1 <= height);

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

		for (uint32_t y = y0; y < y1; ++y)
		{
			for (uint32_t x = x0; x < x1; ++x)
			{
				putPixel(x, y, r, g, b);
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
