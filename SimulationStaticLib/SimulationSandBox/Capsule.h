#pragma once
#include "Shape.h"
class Capsule : public Shape
{
	float _radius{ 0.5f };
	float _height{ 1.0f };

public:

	Capsule(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Material& material, float radius, float height) : Shape(position, rotation,scale,material), _radius(radius), _height(height) {};
	Capsule() = default;
	~Capsule();

	void create() override;
	void move() override;
};

