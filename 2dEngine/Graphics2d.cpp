#include "Graphics2d.h"

namespace eg
{
	Graphics2d::Graphics2d(Window& window, int width, int height)
		: window(window), width(width),
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

	void Graphics2d::draw(const Bitmap& bitmap, int32_t xIn, int32_t yIn)
	{
		int bitmapX = 0, bitmapY = 0, bitmapWidth = bitmap.width, bitmapHeight = bitmap.height;
		if (xIn < 0)
		{
			bitmapX = (0 - xIn) * -1;
			xIn = 0;
		}
		else if (xIn + (int32_t)bitmapWidth >(int32_t)window.width)
		{
			bitmapWidth -= xIn + bitmapWidth - window.width;
		}
		if (yIn < 0)
		{
			bitmapY = (0 - yIn) * -1;
			yIn = 0;
		}
		else if (yIn + (int32_t)bitmapHeight >(int32_t)window.height)
		{
			bitmapHeight -= yIn + bitmapHeight - window.height;
		}

		for (int screenY = yIn, y = bitmapY; screenY < bitmap.height; ++screenY, ++y)
		{
			for (int screenX = xIn, x = bitmapX; screenX < bitmap.width; ++screenX, ++x)
			{
				int32_t *destPixel = (int32_t*)&backBuffer[screenX + screenY * pitch];
				float destR = (float)((*destPixel) && 0xFF);
				float destG = (float)((*destPixel >> 8) & 0xFF);
				float destB = (float)((*destPixel >> 16) & 0xFF);
				float destA = (float)((*destPixel >> 24) & 0xFF);

				float rDestA = destA / 255.0f;

				int32_t *scrPixel = &bitmap.memory[x + y * bitmap.pitch];
				float scrR = (float)((*scrPixel) & 0xFF);
				float scrG = (float)((*scrPixel >> 8) & 0xFF);
				float scrB = (float)((*scrPixel >> 16) & 0xFF);
				float scrA = (float)((*scrPixel >> 24) & 0xFF);

				float rScrA = scrA / 255.0f;

				uint8_t r = (uint8_t)(((1.0f - rScrA)*destR + scrR) + 0.5f);
				uint8_t g = (uint8_t)(((1 - rScrA)*destG + scrG) + 0.5f);
				uint8_t b = (uint8_t)(((1 - rScrA)*destB + scrB) + 0.5f);
				uint8_t a = (uint8_t)(255.0f*(rScrA + rDestA - (rScrA * rDestA)));

				//*destPixel = (destR > 0 ? r : scrR) | ((destG > 0 ? g : scrG) << 8) | ((destB > 0 ? b : scrB) << 16) | (a << 24);
				*destPixel = (r | (g << 8) | (b << 16) | (a << 24));
			}
		}
	}

	void Graphics2d::putPixel(int x, int y, int r, int g, int b)
	{
		backBuffer[x + y * pitch] = (((uint8_t)r) | (((uint8_t)g) << 8) | (((uint8_t)b) << 16));
	}

	void Graphics2d::drawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, int r, int g, int b)
	{
		assert(x0 >= 0 && x0 <= window.width);
		assert(y0 >= 0 && y0 <= window.height);
		assert(x1 >= 0 && x1 <= window.width);
		assert(y1 >= 0 && y1 <= window.height);

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
		HDC dc = GetDC(window.windowHandle);

		PatBlt(dc, 0, 0, window.width, window.height, BLACKNESS);
		StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, (void*)backBuffer, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(window.windowHandle, dc);
	}
}
