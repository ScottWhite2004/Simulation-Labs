#pragma once
#include "Shape.h"
class Plane : public Shape
{
	// Inherited via Shape
	float _width{ 1.0f };
	float _depth{ 1.0f };

public:

	Plane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,const Material& material, float width, float depth) : Shape(position, rotation,scale, material), _width(width), _depth(depth) {};
	Plane() = default;
	~Plane();

	void create() override;
	void move() override;
};

