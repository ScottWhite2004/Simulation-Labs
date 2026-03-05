#pragma once
#include "Shape.h"
class Cylinder : public Shape
{
    std::vector<Vertex> _localVertices;
    std::vector<uint16_t> _localIndices;
    float _radius;
    float _height;
    int _segments;

public:

    Cylinder(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, const SphereCollider& sphereCollider, float mass, const Material& material, const float& radius, const float& height, const int& segments) : Shape(position,rotation,velocity,sphereCollider,mass, material), _radius(radius), _height(height), _segments(segments) {};
    Cylinder() = default;
    ~Cylinder();

    void create() override;
    void move() override;
};

