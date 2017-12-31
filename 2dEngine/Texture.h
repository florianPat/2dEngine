#pragma once

#include "Color.h"
#include <string>

namespace eg
{
	class Texture
	{
	protected:
		int width, height;
		Color* pixels = nullptr;
	protected:
		int leastSignificantSetBit(int bitfield);
		Texture() = default;
	public:
		Texture(const std::string& filename);
		Texture(const Texture& other);
		Texture& operator=(const Texture& other);
		virtual ~Texture();
		int getWidth() const;
		int getHeight() const;
		Color getPixel(int x, int y) const;
	};
}