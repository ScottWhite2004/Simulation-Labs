#pragma once
#include "Collider.h"
#include "Plane.h"
#include "Line.h"
#include "glm/glm.hpp"

	class SphereCollider : public Collider
	{

		float _Radius;
	public:

		SphereCollider(const glm::vec3& position, float radius)
			: _Radius(radius)
		{
			_Position = position;
		}

		virtual bool IsInside(const glm::vec3& point) const override;


		virtual bool Intersects(const Line& line) const override;

		bool CollidesWith(const SphereCollider& other) const;

		bool CollidesWith(const Plane& other) const;

	}; 

