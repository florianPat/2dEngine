#pragma once

#include "Vec2.h"
#include "Texture.h"
#include "Color.h"
#include "Rect.h"

namespace eg
{
	struct Sprite
	{
		Texture& texture;
		Color color = Colors::White;
		Vec2f pos = { 0.0f, 0.0f };
		RectI textureRect;
		/*
		For later:
		Vec2f origin;
		float rotation;
		float scale;
		*/
		Sprite() = default;
		Sprite(Texture& texture, RectI rect = RectI(0, 0, 0, 0));
	};
}