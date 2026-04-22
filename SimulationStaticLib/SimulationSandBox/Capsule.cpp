#include "Capsule.h"

void Capsule::create()
{
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;

	const int longitudeBands = 36;
	const int hemisphereBands = 9;
	const int cylinderBands = 6;

	const float radius = _radius;
	const float totalHeight = std::max(_height, 2.0f * radius);
	const float cylinderHeight = totalHeight - (2.0f * radius);
	const float halfCylinder = 0.5f * cylinderHeight;

	const int latitudeBands = (hemisphereBands * 2) + cylinderBands;

	vertices.reserve((latitudeBands + 1) * (longitudeBands + 1));
	indices.reserve(latitudeBands * longitudeBands * 6);

	const float pi = 3.14159265f;

	for (int lat = 0; lat <= latitudeBands; ++lat) {
		const float t = static_cast<float>(lat) / static_cast<float>(latitudeBands); // [0,1]
		const float y = (-0.5f * totalHeight) + (t * totalHeight);

		float ringRadius = radius;
		glm::vec3 ringCenter(0.0f, 0.0f, 0.0f);

		if (y < -halfCylinder) {
			// Bottom hemisphere
			const float dy = y + halfCylinder; // relative to bottom cap center
			ringRadius = std::sqrt(std::max(0.0f, (radius * radius) - (dy * dy)));
			ringCenter = glm::vec3(0.0f, -halfCylinder, 0.0f);
		}
		else if (y > halfCylinder) {
			// Top hemisphere
			const float dy = y - halfCylinder; // relative to top cap center
			ringRadius = std::sqrt(std::max(0.0f, (radius * radius) - (dy * dy)));
			ringCenter = glm::vec3(0.0f, halfCylinder, 0.0f);
		}

		for (int lon = 0; lon <= longitudeBands; ++lon) {
			const float u = static_cast<float>(lon) / static_cast<float>(longitudeBands);
			const float phi = u * 2.0f * pi;

			const float c = std::cos(phi);
			const float s = std::sin(phi);

			const float x = ringRadius * c;
			const float z = ringRadius * s;

			glm::vec3 localPos(x, y, z);

			glm::vec3 normal;
			if (y < -halfCylinder || y > halfCylinder) {
				normal = glm::normalize(localPos - ringCenter);
			}
			else {
				normal = glm::normalize(glm::vec3(c, 0.0f, s));
			}

			vertices.push_back({
				{ localPos + getPos() },
				{ 1,1,1 },
				{ 1.0f - u, 1.0f - t },
				normal
				});
		}
	}

	for (int lat = 0; lat < latitudeBands; ++lat) {
		for (int lon = 0; lon < longitudeBands; ++lon) {
			const uint16_t first = static_cast<uint16_t>(lat * (longitudeBands + 1) + lon);
			const uint16_t second = static_cast<uint16_t>(first + longitudeBands + 1);

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(static_cast<uint16_t>(first + 1));

			indices.push_back(second);
			indices.push_back(static_cast<uint16_t>(second + 1));
			indices.push_back(static_cast<uint16_t>(first + 1));
		}
	}

	setVertices(vertices);
	setIndices(indices);
}

void Capsule::move()
{
}

Capsule::~Capsule() = default;
