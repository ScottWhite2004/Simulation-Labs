#pragma once
#include <glm/glm.hpp>
#include "Line.h"
class Collider
{

protected:

	glm::vec3 _Position;

public:
	
	virtual bool IsInside(const glm::vec3& point) const
	{ 
		return false;
	}

	virtual bool Intersects(const Line& line) const
	{
		return false;
	}

};

