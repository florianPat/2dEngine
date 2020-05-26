#include "RenderTexture.h"

eg::RenderTexture::RenderTexture(int width, int height)
{
	this->width = width;
	this->height = height;
	this->pixels = new Color[width * height];
}

void eg::RenderTexture::putPixel(int x, int y, const Color & color)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pixels[x + y * width] = color;
}

//TODO: Add clipping
void eg::RenderTexture::draw(const Texture& texture, int xPos, int yPos)
{
	for (int y = 0; y < texture.getHeight(); ++y)
	{
		for (int x = 0; x < texture.getWidth(); ++x)
		{
			Color c;

			Color sc = texture.getPixel(x, y);
			float rScA = sc.getA() / 255.0f;
			Color dc = getPixel(x + xPos, y + yPos);
			float rDcA = dc.getA() / 255.0f;

			c.setR((char)((sc.getR() + (dc.getR() * (1 - rScA))) + 0.5f));
			c.setG((char)((sc.getG() + (dc.getG() * (1 - rScA))) + 0.5f));
			c.setB((char)((sc.getB() + (dc.getB() * (1 - rScA))) + 0.5f));
			c.setA((char)((rScA + rDcA - rScA * rDcA) * 255.0f));

			putPixel(x + (uint)xPos, y + (uint)yPos, c);
		}
	}
}
