#include "Texture.h"
#include <assert.h>
#include <fstream>
#include <Windows.h>

namespace eg
{
	int Texture::leastSignificantSetBit(int bitfield)
	{
		int result = 0;

		while (!(bitfield & 0xf))
			bitfield >>= 1;

		return result;
	}

	Texture::Texture(const std::string & filename)
	{
		std::ifstream file(filename, std::ios::binary);

		assert(file);

		BITMAPFILEHEADER bmFileHeader = {};
		file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

		//NOTE: Would be 'BM' but because endieness stuff...
		assert(bmFileHeader.bfType == 'MB');

		int fileOffsetToPixels = bmFileHeader.bfOffBits;

		BITMAPINFOHEADER bmInfoHeader = {};
		file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

		if (bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32 && bmInfoHeader.biCompression == BI_RGB || bmInfoHeader.biCompression == BI_BITFIELDS)
		{
			int bitCount = bmInfoHeader.biBitCount;

			width = bmInfoHeader.biWidth;
			height = bmInfoHeader.biHeight;

			pixels = new Color[width * height];

			int padding = ((bitCount * width + 31) / 32) * 4;
			bool isBitfield = bmInfoHeader.biCompression == BI_BITFIELDS;

			bool isBottomUp = height < 0;
			if (isBottomUp)
				height *= -1;

			file.seekg(fileOffsetToPixels, std::ios::cur);

			int redMask = 0xff;
			int greenMask = 0xff << 8;
			int blueMask = 0xff << 16;
			int alphaMask = 0xff << 24;

			if (bmInfoHeader.biSize != sizeof(BITMAPINFOHEADER))
			{
				file.seekg(sizeof(bmFileHeader));

				BITMAPV4HEADER bmInfoHeader = {};
				file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

				if (isBitfield)
				{
					redMask = bmInfoHeader.bV4RedMask;
					greenMask = bmInfoHeader.bV4GreenMask;
					blueMask = bmInfoHeader.bV4BlueMask;
					alphaMask = bmInfoHeader.bV4AlphaMask;
				}
			}

			if (isBottomUp)
			{
				for (int y = 0; y < height; ++y)
				{
					for (int x = 0; x < width; ++x)
					{
						int color;
						Color c;

						if (bitCount == 32)
						{
							file.read(reinterpret_cast<char*>(&color), sizeof(color));
							c.SetR((char)(color >> leastSignificantSetBit(redMask)));
							c.SetG((char)(color >> leastSignificantSetBit(greenMask)));
							c.SetB((char)(color >> leastSignificantSetBit(blueMask)));
						}
						else
						{
							c.SetB(file.get());
							c.SetG(file.get());
							c.SetR(file.get());
						}

						assert(x >= 0);
						assert(x < width);
						assert(y >= 0);
						assert(y < height);
						pixels[x + y * width] = c;
					}
					file.seekg(padding, std::ios::cur);
				}
			}
			else
			{
				for (int y = height - 1; y >= 0; --y)
				{
					for (int x = 0; x < width; ++x)
					{
						int color;
						Color c;

						if (bitCount == 32)
						{
							file.read(reinterpret_cast<char*>(&color), sizeof(color));
							c.SetR((char)(color >> leastSignificantSetBit(redMask)));
							c.SetG((char)(color >> leastSignificantSetBit(greenMask)));
							c.SetB((char)(color >> leastSignificantSetBit(blueMask)));
							c.SetA((char)(color >> leastSignificantSetBit(alphaMask)));
						}
						else
						{
							c.SetB(file.get());
							c.SetG(file.get());
							c.SetR(file.get());
							c.SetA(255);
						}

						float rA = c.GetA() / 255.0f;
						c.SetR((char)(c.GetR() * rA + 0.5f));
						c.SetG((char)(c.GetG() * rA + 0.5f));
						c.SetB((char)(c.GetB() * rA + 0.5f));

						assert(x >= 0);
						assert(x < width);
						assert(y >= 0);
						assert(y < height);
						pixels[x + y * width] = c;
					}
					file.seekg(padding, std::ios::cur);
				}
			}
		}
		else if (bmInfoHeader.biCompression == BI_JPEG || bmInfoHeader.biCompression == BI_PNG)
		{
			//TODO: Make this!
			assert(!"InvalidCodePath");
		}
	}

	Texture::Texture(const Texture & other) : width(other.width), height(other.height), pixels(new Color[width * height])
	{
		int nPixels = width * height;
		for (int i = 0; i < nPixels; ++i)
		{
			pixels[i] = other.pixels[i];
		}
	}

	Texture & Texture::operator=(const Texture & other)
	{
		delete[] pixels;
		pixels = nullptr;

		width = other.width;
		height = other.height;

		pixels = new Color[width * height];

		int nPixels = width * height;
		for (int i = 0; i < nPixels; ++i)
		{
			pixels[i] = other.pixels[i];
		}

		return *this;
	}

	Texture::~Texture()
	{
		delete[] pixels;
		pixels = nullptr;
	}

	int Texture::getWidth() const
	{
		return width;
	}

	int Texture::getHeight() const
	{
		return height;
	}

	Color Texture::getPixel(int x, int y) const
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return pixels[x + y * width];
	}
}