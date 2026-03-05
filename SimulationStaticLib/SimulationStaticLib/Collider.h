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

	void SetPosition(const glm::vec3& newPosition) { _Position = newPosition; }
	glm::vec3 GetPosition() const { return _Position; }

};

