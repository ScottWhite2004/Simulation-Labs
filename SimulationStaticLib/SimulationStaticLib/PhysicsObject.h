#pragma once
#include <glm/glm.hpp>
class PhysicsObject
{
	glm::vec3 _position;

	glm::vec3 _rotation;

public:
	PhysicsObject(const glm::vec3& position, const glm::vec3& rotation) : _position(position), _rotation(rotation) {}
	virtual ~PhysicsObject();
	glm::vec3 getPos() const { return _position; }
	glm::vec3 getRot() const { return _rotation; }
	void SetPosition(const glm::vec3& newPosition) { _position = newPosition; }
	void SetRotation(const glm::vec3& newRotation) { _rotation = newRotation; }

};

