#include "Plane.h"

void Plane::create()
{
	const float halfWidth = _width * 0.5f;
	const float halfDepth = _depth * 0.5f;

	setVertices({
		{{ glm::vec3(-halfWidth, 0.0f, -halfDepth) + getPos() }, {1,1,1}, {0,0}, {0,1,0}},
		{{ glm::vec3(halfWidth, 0.0f, -halfDepth) + getPos() }, {1,1,1}, {1,0}, {0,1,0}},
		{{ glm::vec3(halfWidth, 0.0f,  halfDepth) + getPos() }, {1,1,1}, {1,1}, {0,1,0}},
		{{ glm::vec3(-halfWidth, 0.0f,  halfDepth) + getPos() }, {1,1,1}, {0,1}, {0,1,0}},
		});

	setIndices({
		0, 1, 2,
		2, 3, 0
		});
}

void Plane::move()
{
}

Plane::~Plane() = default;
