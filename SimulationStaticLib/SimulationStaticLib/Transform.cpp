#include "pch.h"
#include "Transform.h"

glm::vec3 Transform::getPosition() const
{
	return _position;
}

glm::vec3 Transform::getScale() const
{
	return _scale;
}

glm::quat Transform::getRotation() const
{
	return _rotation;
}

void Transform::setPosition(const glm::vec3& position)
{
	_position = position;
}

void Transform::setScale(const glm::vec3& scale)
{
	_scale = scale;
}

void Transform::setRotation(const glm::quat& rotation)
{
	_rotation = rotation;
}
