#pragma once

#include "Texture.h"
#include "Sprite.h"

namespace eg
{
	class RenderTexture : public Texture
	{
	public:
		RenderTexture(int width, int height);
		void putPixel(int x, int y, const Color& color);
		//TODO: Think about not duplicating all the draw methods form Graphcics (inheritance??)
		void draw(const Texture& texture, int xPos, int yPos);
	};
}