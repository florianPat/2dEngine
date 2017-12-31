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
			float rScA = sc.GetA() / 255.0f;
			Color dc = getPixel(x + xPos, y + yPos);
			float rDcA = dc.GetA() / 255.0f;

			c.SetR((char)((sc.GetR() + (dc.GetR() * (1 - rScA))) + 0.5f));
			c.SetG((char)((sc.GetG() + (dc.GetG() * (1 - rScA))) + 0.5f));
			c.SetB((char)((sc.GetB() + (dc.GetB() * (1 - rScA))) + 0.5f));
			c.SetA((char)((rScA + rDcA - rScA * rDcA) * 255.0f));

			putPixel(x + (uint)xPos, y + (uint)yPos, c);
		}
	}
}
