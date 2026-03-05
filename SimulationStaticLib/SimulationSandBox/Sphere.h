#pragma once
#include "Shape.h"
class Sphere : public Shape
{
    std::vector<Vertex> _localVertices;
    std::vector<uint16_t> _localIndices;
    float _radius;

public:
    Sphere(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, const SphereCollider& sphereCollider, float mass, const Material& material, const float& radius) : Shape(position, rotation, velocity,sphereCollider,mass, material), _radius(radius) {};
	Sphere() = default;
    ~Sphere();
    void create() override;
    void move() override;
    bool WithinBounds(const glm::vec3& point, float buffer = 0.0f) const;
};

