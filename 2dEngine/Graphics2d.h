#pragma once

#include <Windows.h>
#include "Vec2.h"
#include "Rect.h"
#include "Circle.h"

namespace eg
{
	class Graphics2d
	{
		friend class Window;

		HWND& windowHandle;
		BITMAPINFO bitmapInfo = {};
		uint32_t* backBuffer;
		uint32_t bytesPerPixel = 4;
		uint32_t width, height;
		uint32_t pitch;
	private:
		Graphics2d(HWND& windowHandle, int width, int height);
	public:
		~Graphics2d();
		Graphics2d(const Graphics2d&) = delete;
		Graphics2d& operator=(const Graphics2d&) = delete;
		void clear();
		void putPixel(int x, int y, int r, int g, int b);
		void draw(RectI& rect);
		void draw(RectF& rect);
		void draw(FloatCircle& circle);
		void render();
	};
}
