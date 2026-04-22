#pragma once
#include "Shape.h"
class Cuboid : public Shape
{
	// Inherited via Shape
	float _width{ 1.0f };
	float _height{ 1.0f };
	float _depth{ 1.0f };

public:
	Cuboid(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,const Material& material,float width, float height, float depth) : Shape(position, rotation,scale, material), _width(width), _height(height), _depth(depth) {};
	Cuboid() = default;
	~Cuboid();
	
	
	void create() override;
	void move() override;


};

