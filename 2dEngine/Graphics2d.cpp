#include "Graphics2d.h"
#include "Graphics3d.h"

#define VERTEX_ATTRIBUTE_LERP(v1Attribute, v1Lerp, vAt, v0Attribute, v0Lerp) \
	v1Attribute * ((v1Lerp - vAt) / (v1Lerp - v0Lerp)) + v0Attribute * ((vAt - v0Lerp) / (v1Lerp - v0Lerp))

#define VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v1Left, v0Left, v1Right, v0Right, attribute, type, yAt) \
	std::pair<type, type> attribute; \
	attribute.first = VERTEX_ATTRIBUTE_LERP(v1Left.attribute, v1Left.pos.y, yAt, v0Left.attribute, v0Left.pos.y); \
	attribute.second = VERTEX_ATTRIBUTE_LERP(v1Right.attribute, v1Right.pos.y, yAt, v0Right.attribute, v0Right.pos.y);

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
		backBuffer(new uint32_t[width * height]), zBuffer(new float[width * height])
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

	void Graphics2d::drawFlatBottomTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
		const Delegate<Color(const Vertex&, const Material&)>& pixelShader, const Material& material)
	{
		Vector2i clipP0, clipP1;

		float dyLeft = (v1.pos.x - v0.pos.x) / (v1.pos.y - v0.pos.y);
		float dyRight = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
		float dzLeft = (v1.pos.z - v0.pos.z) / (v1.pos.y - v0.pos.y);
		float dzRight = (v2.pos.z - v0.pos.z) / (v2.pos.y - v0.pos.y);

		if (v0.pos.x >= 0 && v0.pos.x < (float)width && v0.pos.y >= 0 && v0.pos.y < (float)height)
		{
			uint32_t zBufferIndex = (uint32_t)v0.pos.y * height + (uint32_t)v0.pos.x;
			if (v0.pos.z > zBuffer[zBufferIndex])
			{
				Vertex interpolatedVertex = {};
				interpolatedVertex.pos = { v0.pos.x, v0.pos.y, 1.0f, 1.0f };
				interpolatedVertex.normal = v0.normal;
				interpolatedVertex.textureCoord = v0.textureCoord;
				interpolatedVertex.color = v0.shadedColor;
				putPixel((int32_t)v0.pos.x, (int32_t)v0.pos.y, pixelShader(interpolatedVertex, material));
				zBuffer[zBufferIndex] = v0.pos.z;
			}
		}

		float x0 = v0.pos.x, x1 = v0.pos.x;
		float zLeft = v0.pos.z, zRight = v0.pos.z;
		for (float i = v0.pos.y + 1; i < v1.pos.y; ++i)
		{
			x0 += dyLeft;
			x1 += dyRight;
			zLeft += dzLeft;
			zRight += dzRight;
			float dzX = (-zLeft + zRight) / (x1 - x0);
			float z = zLeft;
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v1, v0, v2, v0, normal, Vector3f, i);
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v1, v0, v2, v0, textureCoord, Vector2f, i);
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v1, v0, v2, v0, shadedColor, Color, i);

			if (i < 0.0f || i >= height)
				continue;

			for (float x = x0; x < x1; ++x)
			{
				if (x0 >= 0.0f && x1 < width)
				{
					uint32_t zBufferIndex = (uint32_t)i * height + (uint32_t)x;
					if (z > zBuffer[zBufferIndex])
					{
						Vertex interpolatedVertex = {};
						interpolatedVertex.pos = { x, i, 1.0f, 1.0f };
						interpolatedVertex.normal = VERTEX_ATTRIBUTE_LERP(normal.second, x1, x, normal.first, x0);
						interpolatedVertex.textureCoord = VERTEX_ATTRIBUTE_LERP(textureCoord.second, x1, x, textureCoord.first, x0);
						interpolatedVertex.color = VERTEX_ATTRIBUTE_LERP(shadedColor.second, x1, x, shadedColor.first, x0);
						putPixel((int32_t)x, (int32_t)i, pixelShader(interpolatedVertex, material));
						zBuffer[zBufferIndex] = z;
					}
				}

				z += dzX;
			}
		}

		if (v1.pos.y < 0.0f || v1.pos.y >= height)
			return;

		float dzX = (-v1.pos.z + v2.pos.z) / (v2.pos.x - v1.pos.x);
		float z = v1.pos.z;
		for (float x = v1.pos.x; x < v2.pos.x; ++x)
		{
			if (x >= 0.0f && x < width)
			{
				uint32_t zBufferIndex = (uint32_t)v1.pos.y * height + (uint32_t)x;
				if (z > zBuffer[zBufferIndex])
				{
					Vertex interpolatedVertex = {};
					interpolatedVertex.pos = { x, v1.pos.y, 1.0f, 1.0f };
					interpolatedVertex.normal = VERTEX_ATTRIBUTE_LERP(v2.normal, v2.pos.x, x, v1.normal, v1.pos.x);
					interpolatedVertex.textureCoord = VERTEX_ATTRIBUTE_LERP(v2.textureCoord, v2.pos.x, x, v1.textureCoord, v1.pos.x);
					interpolatedVertex.color = VERTEX_ATTRIBUTE_LERP(v2.shadedColor, v2.pos.x, x, v1.shadedColor, v1.pos.x);
					putPixel((uint32_t)x, (uint32_t)v1.pos.y, pixelShader(interpolatedVertex, material));
					zBuffer[zBufferIndex] = z;
				}
			}

			z += dzX;
		}
	}

	void Graphics2d::drawFlatTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2,
		const Delegate<Color(const Vertex&, const Material&)>& pixelShader, const Material& material)
	{
		Vector2i clipP0, clipP1;

		float dyLeft = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);
		float dyRight = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);
		float dzLeft = (v2.pos.z - v0.pos.z) / (v2.pos.y - v0.pos.y);
		float dzRight = (v2.pos.z - v1.pos.z) / (v2.pos.y - v1.pos.y);

		if (v0.pos.y >= 0.0f && v0.pos.y < height)
		{
			float dzX = (-v0.pos.z + v1.pos.z) / (v1.pos.x - v0.pos.x);
			float z = v0.pos.z;
			for (float x = v0.pos.x; x < v1.pos.x; ++x)
			{
				if (x >= 0.0f && x < width)
				{
					uint32_t zBufferIndex = (uint32_t)v0.pos.y * height + (uint32_t)x;
					if (z > zBuffer[zBufferIndex])
					{
						Vertex interpolatedVertex = {};
						interpolatedVertex.pos = { x, v0.pos.y, 1.0f, 1.0f };
						interpolatedVertex.normal = VERTEX_ATTRIBUTE_LERP(v1.normal, v1.pos.x, x, v0.normal, v0.pos.x);
						interpolatedVertex.textureCoord = VERTEX_ATTRIBUTE_LERP(v1.textureCoord, v1.pos.x, x, v0.textureCoord, v0.pos.x);
						interpolatedVertex.color = VERTEX_ATTRIBUTE_LERP(v1.shadedColor, v1.pos.x, x, v0.shadedColor, v0.pos.x);
						putPixel((uint32_t)x, (uint32_t)v0.pos.y, pixelShader(Vertex{}, material));
						zBuffer[zBufferIndex] = z;
					}
				}

				z += dzX;
			}
		}

		float x0 = v0.pos.x, x1 = v1.pos.x;
		float zLeft = v0.pos.z, zRight = v1.pos.z;
		for (float i = v0.pos.y + 1; i < v2.pos.y; ++i)
		{
			x0 += dyLeft;
			x1 += dyRight;
			zLeft += dzLeft;
			zRight += dzRight;
			float dzX = (-zLeft + zRight) / (x1 - x0);
			float z = zLeft;
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v2, v0, v2, v1, normal, Vector3f, i);
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v2, v0, v2, v1, textureCoord, Vector2f, i);
			VERTEX_ATTRIBUTE_LERP_Y_LEFT_AND_RIGHT(v2, v0, v2, v1, shadedColor, Color, i);

			if (i < 0.0f || i >= height)
				continue;

			for (float x = x0; x < x1; ++x)
			{
				if (x0 >= 0.0f && x1 < width)
				{
					uint32_t zBufferIndex = (uint32_t)i * height + (uint32_t)x;
					if (z > zBuffer[zBufferIndex])
					{
						Vertex interpolatedVertex = {};
						interpolatedVertex.pos = { x, i, 1.0f, 1.0f };
						interpolatedVertex.normal = VERTEX_ATTRIBUTE_LERP(normal.second, x1, x, normal.first, x0);
						interpolatedVertex.textureCoord = VERTEX_ATTRIBUTE_LERP(textureCoord.second, x1, x, textureCoord.first, x0);
						interpolatedVertex.color = VERTEX_ATTRIBUTE_LERP(shadedColor.second, x1, x, shadedColor.first, x0);
						putPixel((int32_t)x, (int32_t)i, pixelShader(interpolatedVertex, material));
						zBuffer[zBufferIndex] = z;
					}
				}

				z += dzX;
			}
		}

		if (v2.pos.x >= 0 && v2.pos.x < (float)width && v2.pos.y >= 0 && v2.pos.y < (float)height)
		{
			uint32_t zBufferIndex = (uint32_t)v2.pos.y * height + (uint32_t)v2.pos.x;
			if (v2.pos.z > zBuffer[zBufferIndex])
			{
				Vertex interpolatedVertex = {};
				interpolatedVertex.pos = { v2.pos.x, v2.pos.y, 1.0f, 1.0f };
				interpolatedVertex.normal = v2.normal;
				interpolatedVertex.textureCoord = v2.textureCoord;
				interpolatedVertex.color = v2.shadedColor;
				putPixel((int32_t)v2.pos.x, (int32_t)v2.pos.y, pixelShader(interpolatedVertex, material));
				zBuffer[zBufferIndex] = v2.pos.z;
			}
		}
	}

	Graphics2d::~Graphics2d()
	{
		delete[] backBuffer;
		backBuffer = nullptr;

		delete[] zBuffer;
		zBuffer = nullptr;
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

		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				zBuffer[x + y * pitch] = 0;
			}
		}
	}

	void Graphics2d::putPixel(int x, int y, Color c)
	{
		backBuffer[x + y * pitch] = c.color;
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
					sc.setR((uchar)(Lerp(Lerp(texelA.getR() / 255.0f, fX, texelB.getR() / 255.0f), fY, Lerp(texelC.getR() / 255.0f, fX, texelD.getR() / 255.0f)) * 255));
					sc.setG((uchar)(Lerp(Lerp(texelA.getG() / 255.0f, fX, texelB.getG() / 255.0f), fY, Lerp(texelC.getG() / 255.0f, fX, texelD.getG() / 255.0f)) * 255));
					sc.setB((uchar)(Lerp(Lerp(texelA.getB() / 255.0f, fX, texelB.getB() / 255.0f), fY, Lerp(texelC.getB() / 255.0f, fX, texelD.getB() / 255.0f)) * 255));
					sc.setA((uchar)(Lerp(Lerp(texelA.getA() / 255.0f, fX, texelB.getA() / 255.0f), fY, Lerp(texelC.getA() / 255.0f, fX, texelD.getA() / 255.0f)) * 255));

					float rScA = sc.getA() / 255.0f;
					
					Color dc = getPixel(x + (int)sprite.pos.x, y + (int)sprite.pos.y);
					float rDcA = dc.getA() / 255.0f;

					c.setR((char)((sc.getR() + (dc.getR() * (1 - rScA))) + 0.5f));
					c.setG((char)((sc.getG() + (dc.getG() * (1 - rScA))) + 0.5f));
					c.setB((char)((sc.getB() + (dc.getB() * (1 - rScA))) + 0.5f));
					c.setA((char)((rScA + rDcA - rScA * rDcA) * 255.0f));

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
	void Graphics2d::drawTriangle(const Vertex* vertices, const Delegate<Color(const Vertex&, const Material&)>& pixelShader,
		const Material& material)
	{
		const Vertex* v0 = vertices, * v1 = &vertices[1], * v2 = &vertices[2];

		const Vertex* temp;

		if (v0->pos.y > v2->pos.y)
		{
			temp = v2;
			v2 = v0;
			v0 = temp;
		}
		if (v0->pos.y > v1->pos.y)
		{
			temp = v1;
			v1 = v0;
			v0 = temp;
		}
		if (v1->pos.y > v2->pos.y)
		{
			temp = v2;
			v2 = v1;
			v1 = temp;
		}

		if (v0->pos.y == v1->pos.y)
		{
			if (v0->pos.x > v1->pos.x)
			{
				temp = v1;
				v1 = v0;
				v0 = temp;
			}

			drawFlatTopTriangle(*v0, *v1, *v2, pixelShader, material);
		}
		else if (v1->pos.y == v2->pos.y)
		{
			if (v1->pos.x > v2->pos.x)
			{
				temp = v2;
				v2 = v1;
				v1 = temp;
			}

			drawFlatBottomTriangle(*v0, *v1, *v2, pixelShader, material);
		}
		else
		{
			if (v1->pos.x > v0->pos.x)
			{
				float dyLeft = (v2->pos.x - v0->pos.x) / (v2->pos.y - v0->pos.y);
				
				Vertex realV2 = {};
				float z = VERTEX_ATTRIBUTE_LERP(v1->pos.z, v1->pos.y, realV2.pos.z, v0->pos.z, v0->pos.y);
				realV2.pos = { dyLeft * (v1->pos.y - v0->pos.y) + v0->pos.x, v1->pos.y, 1.0f, z };
				realV2.normal = VERTEX_ATTRIBUTE_LERP(v1->normal, v1->pos.y, realV2.pos.y, v0->normal, v0->pos.y);
				realV2.normal.normalize();
				realV2.textureCoord = VERTEX_ATTRIBUTE_LERP(v1->textureCoord, v1->pos.y, realV2.pos.y, v0->textureCoord, v0->pos.y);
				realV2.shadedColor = VERTEX_ATTRIBUTE_LERP(v1->shadedColor, v1->pos.y, realV2.pos.y, v0->shadedColor, v0->pos.y);
				
				drawFlatBottomTriangle(*v0, realV2, *v1, pixelShader, material);
				drawFlatTopTriangle(realV2, *v1, *v2, pixelShader, material);
			}
			else
			{
				float dyRight = (v2->pos.x - v0->pos.x) / (v2->pos.y - v0->pos.y);

				Vertex realV2 = {};
				float z = VERTEX_ATTRIBUTE_LERP(v1->pos.z, v1->pos.y, realV2.pos.y, v0->pos.z, v0->pos.y);
				// NOTE: x can also be computed as follows:
				// float x = VERTEX_ATTRIBUTE_LERP(v2->pos.x, v2->pos.y, v1->pos.y, v0->pos.x, v0->pos.y);
				realV2.pos = { dyRight * (v1->pos.y - v0->pos.y) + v0->pos.x, v1->pos.y, 1.0f, z };
				realV2.normal = VERTEX_ATTRIBUTE_LERP(v1->normal, v1->pos.y, realV2.pos.y, v0->normal, v0->pos.y);
				realV2.normal.normalize();
				realV2.textureCoord = VERTEX_ATTRIBUTE_LERP(v1->textureCoord, v1->pos.y, realV2.pos.y, v0->textureCoord, v0->pos.y);
				realV2.shadedColor = VERTEX_ATTRIBUTE_LERP(v1->shadedColor, v1->pos.y, realV2.pos.y, v0->shadedColor, v0->pos.y);

				drawFlatBottomTriangle(*v0, *v1, realV2, pixelShader, material);
				drawFlatTopTriangle(*v1, realV2, *v2, pixelShader, material);
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
