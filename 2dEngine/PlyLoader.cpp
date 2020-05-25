#include "PlyLoader.h"
#include <fstream>

#define readLocalVertexElement(coord) \
{ \
	switch (property.type) \
	{ \
		case Property::Type::FLOAT: \
		{ \
			vertex.coord = (float)atof(line.substr(0, line.find(' ')).c_str()); \
			break; \
		} \
		case Property::Type::INT: \
		{ \
			vertex.coord = (float)atoi(line.substr(0, line.find(' ')).c_str()); \
			break; \
		} \
	} \
	line.erase(0, line.find(' ') + (k == (element.properties.size() - 1) ? 0 : 1)); \
	break; \
}

namespace eg
{
	Object eg::PlyLoader::load(const std::string& filename, const Vector3f& pos, const Vector3f& rot, const Vector3f& scl)
	{
		Object result;
		result.worldPos = pos;
		result.rot = rot;
		result.scl = scl;

		std::ifstream file(filename);
		assert(file);

		std::string line;
		std::getline(file, line);
		assert(line == "ply");

		std::getline(file, line);
		assert(line.find("format ") == 0);
		line.erase(0, 7);
		line.erase(line.find(" 1.0"));

		if (line != "ascii")
			utilsLogBreak("Only ascii implemented currently");

		std::vector<Element> elements;

		std::getline(file, line);
		while (line.find("end_header") == std::string::npos)
		{
			if (line.find("comment") != std::string::npos)
			{
				std::getline(file, line);
				continue;
			}

			assert(line.find("element") != std::string::npos);

			Element element;

			if (line.find("vertex") != std::string::npos)
			{
				line.erase(0, 15);

				element.name = Element::Name::VERTEX_BUFFER;
				element.valueCount = atoi(line.c_str());

				for (std::getline(file, line); line.find("property") != std::string::npos; std::getline(file, line))
				{
					line.erase(0, line.find(' ') + 1);

					Property property;
						
					property.type = Property::Type::INT;
					if (line.find("float") != std::string::npos || line.find("double") != std::string::npos)
					{
						property.type = Property::Type::FLOAT;
					}
						
					line.erase(0, line.find(' ') + 1);

					property.name[0] = line[0];
					if (line.size() > 1)
					{
						property.name[1] = line[1];
					}

					element.properties.push_back(property);
				}
			}
			else if (line.find("face") != std::string::npos)
			{
				line.erase(0, 13);

				element.name = Element::Name::INDEX_BUFFER;
				element.valueCount = atoi(line.c_str());
				result.nPolygons = element.valueCount;

				std::getline(file, line);

				assert(line.find("property") != std::string::npos);
				assert(line.find("list") != std::string::npos);
				assert(line.find("float") == std::string::npos);

				std::getline(file, line);
				assert(line.find("property") == std::string::npos);
			}
			else
			{
				utilsLogBreak("Not recognized element");
			}

			elements.push_back(element);
		}
		std::getline(file, line);

		std::vector<Vertex> vertices;
		std::vector<uint16_t> indicies;
		indicies.reserve(result.nPolygons);

		for (const auto& element : elements)
		{
			switch (element.name)
			{
				case Element::Name::VERTEX_BUFFER:
				{
					uint32_t j = 0;
					for (; j < element.valueCount; ++j, std::getline(file, line))
					{
						Vertex vertex;

						for (uint32_t k = 0; k < element.properties.size(); ++k)
						{
							const auto& property = element.properties[k];

							switch (property.name[0])
							{
								case 'x': readLocalVertexElement(pos.x)
								case 'y': readLocalVertexElement(pos.y)
								case 'z': readLocalVertexElement(pos.z)

								case 'n':
								{
									readLocalVertexElement(normal.x)
									readLocalVertexElement(normal.y)
									readLocalVertexElement(normal.z)
									break;
								}
								case 's':
								{
									readLocalVertexElement(textureCoord.x)
									break;
								}
								case 't':
								{
									readLocalVertexElement(textureCoord.y)
									break;
								}
							}
						}

						vertices.push_back(vertex);
					}
					break;
				}
				case Element::Name::INDEX_BUFFER:
				{
					uint32_t j = 0;
					for (; j < element.valueCount; ++j, std::getline(file, line))
					{
						assert(line[0] == '3');
						line.erase(0, 2);
						for (int k = 0; k < 3; ++k)
						{
							indicies.push_back(atoi(line.substr(0, line.find(' ')).c_str()));
							line.erase(0, line.find(' ') + (k == 2 ? 0 : 1));
						}
					}
				}
			}
		}

		for (uint16_t i = 0; i < indicies.size();)
		{
			result.polygons[i++ / 3].localCoords[0] = vertices[indicies[i]];
			result.polygons[i++ / 3].localCoords[1] = vertices[indicies[i]];
			result.polygons[i++ / 3].localCoords[2] = vertices[indicies[i]];
		}

		return result;
	}
}
