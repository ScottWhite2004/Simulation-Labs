#pragma once
#include "glm/glm.hpp"

struct CollisionEvent
{
	float timeOfImpact{ 0.0f };
	glm::vec3 collisionNormal{ 0.0f, 1.0f, 0.0f };
	glm::vec3 collisionPoint{ 0.0f };
	float penetrationDepth{ 0.0f };
	bool isColliding{ false };
};

