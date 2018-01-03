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
		RectI textureRect = { 0, 0, 0, 0 };
		Vec2f origin = { 0.0f, 0.0f };
		float rotation = 0.0f;
		//TODO: Add scaling
		float scale = 1.0f;
	public:
		Sprite() = default;
		Sprite(Texture& texture, RectI rect = RectI(0, 0, 0, 0));
	};
}