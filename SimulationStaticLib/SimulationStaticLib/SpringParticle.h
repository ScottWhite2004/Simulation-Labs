#pragma once
#include "PhysicsObject.h"
class SpringParticle : public PhysicsObject
{
	glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 _velocity{ 0.0f, 0.0f, 0.0f };

public:
	SpringParticle() = default;
	SpringParticle(const glm::vec3& position, const glm::vec3& velocity, float mass, bool isStatic = false) : _position(position), _velocity(velocity)
	{
		SetMass(mass);
		SetStatic(isStatic);
	}

	glm::vec3 getPos() const { return _position; }
	glm::vec3 getVel() const { return _velocity; }

	void setPos(const glm::vec3& position) { _position = position; }
	void setVel(const glm::vec3& velocity) { _velocity = velocity; }

	void IntegrateEuler(float seconds) override;
	void IntegrateSemiImplicitEuler(float seconds) override;
};

