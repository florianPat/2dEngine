#pragma once

#include "Color.h"
#include <string>

namespace eg
{
	class Texture
	{
	protected:
		int32_t width = 0, height = 0;
		Color* pixels = nullptr;
	protected:
		int leastSignificantSetBit(int bitfield);
	private:
		void loadBmp(const std::string& filename);
		void loadPng(const std::string& filename);
		void swapEndian(uint& value);
	public:
		Texture() = default;
		Texture(const std::string& filename);
		Texture(const Texture& other);
		Texture& operator=(const Texture& other);
		virtual ~Texture();
		int getWidth() const;
		int getHeight() const;
		Color getPixel(int x, int y) const;
	};
}