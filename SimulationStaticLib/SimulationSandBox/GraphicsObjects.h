#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class GraphicsObjects
{
	glm::vec3 position{ 0.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f };

public:
	GraphicsObjects(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
		: position(position), rotation(rotation), scale(scale) {
	}

	GraphicsObjects(const glm::vec3& position, const glm::vec3& rotationEulerRadians, const glm::vec3& scale)
		: position(position), rotation(glm::quat(rotationEulerRadians)), scale(scale) {
	}

	GraphicsObjects() = default;
	~GraphicsObjects() = default;

	glm::vec3 getPos() const { return position; }
	glm::quat getRot() const { return rotation; }
	glm::vec3 getScale() const { return scale; }

	void setPosition(const glm::vec3& pos) { position = pos; }
	void setRotation(const glm::quat& rot) { rotation = glm::normalize(rot); }
	void setRotation(const glm::vec3& rotEulerRadians) { rotation = glm::quat(rotEulerRadians); }
	void setScale(const glm::vec3& scl) { scale = scl; }
};

