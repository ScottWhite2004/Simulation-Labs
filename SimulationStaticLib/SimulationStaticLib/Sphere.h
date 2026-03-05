#pragma once
#include "Collider.h"
#include "Plane.h"
#include "Line.h"
#include "glm/glm.hpp"

	class Sphere : public Collider
	{

		float _Radius;
	public:

		Sphere(const glm::vec3& position, float radius)
			: _Radius(radius)
		{
			_Position = position;
		}

		virtual bool IsInside(const glm::vec3& point) const override;


		virtual bool Intersects(const Line& line) const override;

		bool CollidesWith(const Sphere& other) const;

		bool CollidesWith(const Plane& other) const;

	}; 

