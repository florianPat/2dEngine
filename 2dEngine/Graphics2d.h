#pragma once

#include <Windows.h>
#include "Vector2.h"
#include "Rect.h"
#include "Circle.h"
#include "Sprite.h"
#include "Color.h"
#include "ORect.h"

namespace eg
{
	class Graphics2d
	{
		friend class Window;

		HWND& windowHandle;
		BITMAPINFO bitmapInfo = {};
		uint32_t* backBuffer;
		uint32_t bytesPerPixel = 4;
	public:
		const uint32_t width, height;
	private:
		uint32_t pitch;
	private:
		Graphics2d(HWND& windowHandle, int width, int height);
		void ClipRect(int& x, int& y);
	public:
		~Graphics2d();
		Graphics2d(const Graphics2d&) = delete;
		Graphics2d& operator=(const Graphics2d&) = delete;
		void clear();
		void putPixel(int x, int y, Color c);
		Color getPixel(uint x, uint y);
		void draw(RectI& rect);
		void draw(RectF& rect);
		void draw(FloatCircle& circle);
		void draw(const Sprite& sprite);
		void draw(const ORectF& oRect);
		void render();
	};
}
