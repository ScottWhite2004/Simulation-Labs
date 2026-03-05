#pragma once
#include "Shape.h"
class Cube : public Shape
{

public:
	Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, const SphereCollider& sphereCollider, float mass, const Material& material) : Shape(position, rotation, velocity, sphereCollider, mass, material) {};
	Cube() = default;
	~Cube();

	void create() override;
	void move() override;
};

