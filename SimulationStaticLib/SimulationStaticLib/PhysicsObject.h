#pragma once
#include <glm/glm.hpp>
#include "Collider.h"
class PhysicsObject
{
	glm::vec3 _position;

	glm::vec3 _rotation;

	Collider _collider;

public:
	PhysicsObject(const glm::vec3& position, const glm::vec3& rotation, const Collider& collider) : _position(position), _rotation(rotation), _collider(collider) {}
	virtual ~PhysicsObject();
	glm::vec3 getPos() const { return _position; }
	glm::vec3 getRot() const { return _rotation; }
	void SetPosition(const glm::vec3& newPosition) { _position = newPosition; }
	void SetRotation(const glm::vec3& newRotation) { _rotation = newRotation; }

};

