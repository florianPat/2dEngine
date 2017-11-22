#include "Bitmap.h"

#include <fstream>

namespace eg
{
	uint32_t leastSignificantSetBit(uint32_t mask)
	{
		uint32_t result = 0;

		//for(;!(mask & 0x1); mask >>= 1, ++result);
		while (!(mask & 0x1))
		{
			mask >>= 1;
			++result;
		}

		return result;
	}

	Bitmap::Bitmap(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (file)
		{
			FileHeader fileHeader = {};
			file.read(reinterpret_cast<char*>(&fileHeader), sizeof(FileHeader));

			assert(fileHeader.signature == 0x4D42);

			BitmapInfoHeaderV3 infoHeader = {};
			file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BitmapInfoHeaderV3));

			width = infoHeader.width;
			height = infoHeader.height;

			assert(infoHeader.bitCount == 24 || infoHeader.bitCount == 32);

			//TODO: Implement JPEG / PNG
			assert(infoHeader.compression == BI_RGB ||
				infoHeader.compression == BI_BITFIELDS ||
				infoHeader.compression == BI_JPEG ||
				infoHeader.compression == BI_PNG);


			int32_t redMask = 0xFF;
			int32_t greenMask = 0xFF << 8;
			int32_t blueMask = 0xFF << 16;
			int32_t alphaMask = 0xFF << 24;

			if (infoHeader.size == sizeof(BitmapInfoHeaderV4))
			{
				file.seekg(sizeof(fileHeader));

				BitmapInfoHeaderV4 infoHeaderV4 = {};
				file.read(reinterpret_cast<char*>(&infoHeaderV4), sizeof(BitmapInfoHeaderV4));

				redMask = infoHeaderV4.redMask;
				greenMask = infoHeaderV4.greenMask;
				blueMask = infoHeaderV4.blueMask;
				alphaMask = infoHeaderV4.alphaMask;
			}

			file.seekg(fileHeader.fileOffsetToPixelArray);

			pitch = (((infoHeader.bitCount == 24 ? 24 : 32) * width + 31) / 32) * 4;

			memory = new int32_t[width * height];

			int32_t *pixelMemory = memory;
			for (int i = 0; i < height; ++i)
			{
				int32_t *row = pixelMemory;
				for (int j = 0; j < width; ++j)
				{
					int32_t color;
					if (infoHeader.bitCount == 32)
						file.read((char*)&color, sizeof(int32_t));
					else
						color = file.get() | (file.get() << 8) | (file.get() << 16);

					int8_t r = (int8_t)((color | redMask));// >> leastSignificantSetBit(redMask));
					int8_t g = (int8_t)((color | greenMask));// >> leastSignificantSetBit(greenMask));
					int8_t b = (int8_t)((color | blueMask));// >> leastSignificantSetBit(blueMask));
					int8_t a = infoHeader.bitCount == 32 ? (uint8_t)((color | alphaMask) >> leastSignificantSetBit(alphaMask)) : 255;

					//NOTE: Here premultiplied alpha
					float aReal = a / 255.0f;
					r = (int8_t)(r * aReal);
					g = (int8_t)(g * aReal);
					b = (int8_t)(b * aReal);

					*row++ = (r | (g << 8) | (b << 16) | (a << 24));
				}

				pixelMemory += pitch;
			}
		}
		else
		{
			//TODO: Logging!
			InvalidCodePath;
		}
	}
	Bitmap::~Bitmap()
	{
		delete [] memory;
	}
}