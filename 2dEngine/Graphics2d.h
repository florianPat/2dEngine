#pragma once

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include "Vector2.h"
#include "Rect.h"
#include "Circle.h"
#include "Sprite.h"
#include "Color.h"
#include "ORect.h"
#include <vector>
#include "Delegate.h"

namespace eg
{
	// TODO: Think about moving drawTriangle into Graphics3d!
	struct Vertex;
	struct Material;

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
		float* zBuffer;
	private:
		void ClipRect(int& x, int& y);
		void drawFlatBottomTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
			const Delegate<Color(const Vertex&, const Material&)>& pixelShader, const Material& material);
		void drawFlatTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
			const Delegate<Color(const Vertex&, const Material&)>& pixelShader, const Material& material);
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
		void drawTriangle(const Vertex* vertices, const Delegate<Color(const Vertex&, const Material&)>& pixelShader,
			const Material& material);
		void render();
	};
}
