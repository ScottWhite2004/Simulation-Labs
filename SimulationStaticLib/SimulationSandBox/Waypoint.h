#pragma once
#include "glm/glm.hpp"
#include "RotationEuler.h"
class Waypoint
{
	glm::vec3 position;
	RotationEuler rotation;
	float time;
};

