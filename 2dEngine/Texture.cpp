#include "Texture.h"
#include <assert.h>
#include <fstream>
#include <Windows.h>
#include "PngLoadStructs.h"

namespace eg
{
	int Texture::leastSignificantSetBit(int bitfield)
	{
		int result = 0;

		while (!(bitfield & 0xf))
			bitfield >>= 1;

		return result;
	}

	void Texture::loadBmp(const std::string & filename)
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

			bool isBottomUp = height < 0;
			if (isBottomUp)
				height *= -1;

			pixels = new Color[width * height];

			int padding = ((bitCount * width + 31) / 32) * 4;
			bool isBitfield = bmInfoHeader.biCompression == BI_BITFIELDS;

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
							c.setR((char)(color >> leastSignificantSetBit(redMask)));
							c.setG((char)(color >> leastSignificantSetBit(greenMask)));
							c.setB((char)(color >> leastSignificantSetBit(blueMask)));
						}
						else
						{
							c.setB(file.get());
							c.setG(file.get());
							c.setR(file.get());
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
							c.setR((char)(color >> leastSignificantSetBit(redMask)));
							c.setG((char)(color >> leastSignificantSetBit(greenMask)));
							c.setB((char)(color >> leastSignificantSetBit(blueMask)));
							c.setA((char)(color >> leastSignificantSetBit(alphaMask)));
						}
						else
						{
							c.setB(file.get());
							c.setG(file.get());
							c.setR(file.get());
							c.setA(255);
						}

						float rA = c.getA() / 255.0f;
						c.setR((char)(c.getR() * rA + 0.5f));
						c.setG((char)(c.getG() * rA + 0.5f));
						c.setB((char)(c.getB() * rA + 0.5f));

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
			assert(!"InvalidCodePath");
		}
	}

	void Texture::loadPng(const std::string & filename)
	{
		std::ifstream file(filename, std::ios::binary);

		assert(file);

		PngLoad::FileHeader fileHeader = {};
		file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));

		while (!file.eof())
		{
			PngLoad::ChunkHeader chunkHeader = {};
			file.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));

			//NOTE: Is needed because png-files are in not our endienness
			swapEndian(chunkHeader.size);
			char* chunkData = new char[chunkHeader.size];
			file.read(reinterpret_cast<char*>(&chunkData), chunkHeader.size);

			ChunkFooter chunkFooter;
			file.read(reinterpret_cast<char*>(&chunkFooter), sizeof(chunkFooter));

			switch (chunkHeader.type)
			{
				case FOURCC("IHDR"):
				{
					std::cout << "Beginning!!" << '\n';
				}
			}
		}
	}

	void Texture::swapEndian(uint & value)
	{
#if 0
		uint v0 = value & 0xFF;
		uint v1 = (value >> 8) & 0xFF;
		uint v2 = (value >> 16) & 0xFF;
		uint v3 = (value >> 24) & 0xFF;

		value = ((v3) | (v2 << 8) | (v1 << 16) | (v0 << 24));
#else
		uint v = value;
		value = ((v << 24) | ((v & 0xFF00) << 8) | ((v >> 8) & 0xFF00) | (v >> 24));
#endif
	}

	Texture::Texture(const std::string & filename)
	{
		std::string extension = filename;
		extension = extension.erase(extension.find('.'));

		if (extension == "bmp")
		{
			loadBmp(filename);
		}
		else if (extension == "png")
		{
			loadPng(filename);
		}
		else
		{
			assert(!"InvalidCodePath!");
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
		if (pixels != nullptr)
		{
			delete[] pixels;
			pixels = nullptr;
		}
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