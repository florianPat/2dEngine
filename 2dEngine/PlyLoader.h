#pragma once

#include "Graphics3d.h"

namespace eg
{
	struct PlyLoader
	{
		struct Property
		{
			enum class Type
			{
				INT,
				FLOAT,
			} type;

			char name[2];
		};

		struct Element
		{
			enum class Name
			{
				VERTEX_BUFFER,
				INDEX_BUFFER,
			} name;
			std::vector<Property> properties;
			uint32_t valueCount;
		};

		static Object load(const std::string& filename, const Vector3f& pos, const Vector3f& rot, const Vector3f& scl);
	};
}