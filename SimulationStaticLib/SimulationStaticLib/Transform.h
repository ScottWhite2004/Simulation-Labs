#pragma once
#include "glm/glm.hpp"
#include"glm/gtc/quaternion.hpp"
class Transform
{
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::quat _rotation;


public:
	Transform(const glm::vec3& position, const glm::vec3& scale, const glm::quat rotation) : _position(position), _scale(scale), _rotation(rotation) {};
	Transform() : _position(0.0f), _scale(1.0f), _rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}
	~Transform() = default;

	glm::vec3 getPosition() const;
	glm::quat getRotation() const;
	glm::vec3 getScale() const;

	void setPosition(const glm::vec3& position);
	void setRotation(const glm::quat& rotation);
	void setScale(const glm::vec3& scale);


};

