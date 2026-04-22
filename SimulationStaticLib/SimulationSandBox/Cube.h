#pragma once
#include "Shape.h"
class Cube : public Shape
{

public:
	Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, const Material& material) : Shape(position, rotation,scale,material) {};
	Cube() = default;
	~Cube();

	void create() override;
	void move() override;
};

