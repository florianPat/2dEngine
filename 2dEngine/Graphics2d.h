#pragma once

#include <Windows.h>
#include "Vector2.h"
#include "Rect.h"
#include "Circle.h"
#include "Sprite.h"
#include "Color.h"
#include "ORect.h"
#include <vector>

namespace eg
{
	class Graphics2d
	{
		HWND& windowHandle;
		BITMAPINFO bitmapInfo = {};
		uint32_t* backBuffer;
		uint32_t bytesPerPixel = 4;
	public:
		const uint32_t width, height;
	private:
		uint32_t pitch;
	private:
		void ClipRect(int& x, int& y);
		void drawFlatBottomTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, Color color);
		void drawFlatTopTriangle(const Vector2i& p0, const Vector2i& p1, const Vector2i& p2, Color color);
	public:
		Graphics2d(HWND& windowHandle, int width, int height);
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
		void drawLine(const Vector2i& p0, const Vector2i& p1, Color color);
		bool clipLine(Vector2i& p0, Vector2i& p1);
		void drawPolyline(std::vector<Vector2i> points, Color color);
		void drawTriangle(Vector2i p0, Vector2i p1, Vector2i p2, Color color);
		void render();
	};
}
