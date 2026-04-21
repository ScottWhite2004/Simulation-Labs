#include "pch.h"
#include "PhysicsObject.h"

PhysicsObject::PhysicsObject() = default;

void PhysicsObject::IntegrateEuler(float)
{
}

void PhysicsObject::IntegrateSemiImplicitEuler(float)
{
}


//void PhysicsObject::ResolveSphereSphereCollision(PhysicsObject& other)
//{
//	float distance = glm::length(other.getPos() - _position);
//	glm::vec3 normal = glm::normalize(other.getPos() - _position);
//	if (distance == 0.0f)
//	{
//		normal = glm::vec3(1.0f, 0.0f, 0.0f); // Arbitrary normal if spheres are at the same position
//	}
//	glm::vec3 relativeVelocity = _velocity - other.getVel();
//	float velAlongNormal = glm::dot(relativeVelocity, normal);
//
//	if (velAlongNormal <= 0.0f)
//		return;
//
//	float e = 1.0f; // coefficient of restitution
//
//	float m1 = _mass;
//	float m2 = other.getMass();
//
//	float j = -(1.0f + e) * velAlongNormal;
//	j /= (1.0f / m1 + 1.0f / m2);
//
//	glm::vec3 impulse = j * normal;
//
//	_velocity += impulse / m1;
//	other.SetVelocity(other.getVel() - impulse / m2);
//}

