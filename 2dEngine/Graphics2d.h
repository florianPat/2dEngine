#pragma once

#include "window.h"
#include "Bitmap.h"

namespace eg
{
	class Graphics2d
	{
		Window& window;
		BITMAPINFO bitmapInfo = {};
		uint32_t* backBuffer;
		uint32_t bytesPerPixel = 4;
		uint32_t width, height;
		uint32_t pitch;
	public:
		Graphics2d(Window& window, int width, int height);
		~Graphics2d();
		Graphics2d(const Graphics2d&) = delete;
		void clear();
		void draw(const Bitmap& bitmap, int32_t x, int32_t y);
		void putPixel(int x, int y, int r, int g, int b);
		void drawRect(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, int r, int g, int b);
		void render();
	};
}
