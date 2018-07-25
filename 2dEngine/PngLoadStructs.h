#pragma once

namespace PngLoad
{
	struct FileHeader
	{
		uchar signiture;
	};

	struct ChunkHeader
	{
		uint size;
		uint type;
	};

#define ChunkFooter uint

//NOTE: String of four into bytei-style
#define FOURCC(string) (uint)((((uint)string[0]) << 24) | (((uint)string[1]) << 16) | (((uint)string[2]) << 8) | ((uint)string[3]))
}