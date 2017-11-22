#pragma once

#include "window.h"

namespace eg
{
	class Bitmap
	{
		friend class Graphics2d;
#pragma pack(push, 1)
		struct FileHeader
		{
			uint16_t signature;
			uint32_t fileSize;
			uint16_t reserved1, reserved2;
			uint32_t fileOffsetToPixelArray;
		};
		struct BitmapInfoHeaderV3
		{
			uint32_t size;
			int32_t width;
			int32_t height;
			uint16_t planes;
			uint16_t bitCount;
			uint32_t compression;
			uint32_t biSizeImage;
			int32_t  biXPelsPerMeter;
			int32_t  biYPelsPerMeter;
			uint32_t biClrUsed;
			uint32_t biClrImportant;
		};
		struct BitmapInfoHeaderV4
		{
			uint32_t bV4Size;
			int32_t bV4Width;
			int32_t bV4Height;
			uint16_t bV4Planes;
			uint16_t bV4BitCount;
			uint32_t bV4V4Compression;
			uint32_t bV4SizeImage;
			int32_t bV4XPelsPerMeter;
			int32_t bV4YPelsPerMeter;
			uint32_t bV4ClrUsed;
			uint32_t bV4ClrImportant;
			uint32_t redMask;
			uint32_t greenMask;
			uint32_t blueMask;
			uint32_t alphaMask;
			uint32_t bV4CSType;
		};
#pragma pack(pop, 1)
		int32_t width, height;
		int32_t bytesPerPixel = 4;
		int32_t pitch;
		int32_t *memory;
	public:
		Bitmap(const std::string& filename);
		~Bitmap();
	};
}