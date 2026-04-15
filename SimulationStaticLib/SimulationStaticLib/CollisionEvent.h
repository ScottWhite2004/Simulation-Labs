#pragma once
#include "glm/glm.hpp"
class CollisionEvent
{
	float timeOfImpact;
	glm::vec3 collisionNormal;
	glm::vec3 collisionPoint;
	bool isColliding;
};

