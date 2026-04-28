#pragma once
#include "PhysicsObject.h"
class RigidBody : public PhysicsObject
{
	Transform _transform;

	glm::vec3 _velocity{ 0 };
	glm::vec3 _angularVelocity{ 0 };

	glm::vec3 _torque{ 0 };
	glm::vec3 _angularAcceleration{ 0 };

	glm::vec3 _inertia = glm::vec3(1.0f);
	glm::vec3 _inverseInertia = glm::vec3(1.0f);

public:
	RigidBody();
	RigidBody(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, float mass);

	void addAngularDisplacement(const glm::vec3& axis, const float& radians);
	void addAngularDisplacement(const glm::quat& angularDisplacement);
	void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
	void IntegrateEuler(float seconds) override;
	void IntegrateSemiImplicitEuler(float seconds) override;
	void CalculateInertia();
	void clearForces() override;
	
	//Getters
 glm::vec3 getPos() const { return _transform.getPosition(); }
   glm::quat getOrientation() const { return _transform.getRotation(); }
	Transform getTransform() const { return _transform; }
	glm::vec3 getVelocity() const { return _velocity; }
	glm::vec3 getAngularVelocity() const { return _angularVelocity; }
	glm::vec3 getTorque() const { return _torque; }
	glm::vec3 getAngularAcceleration() const { return _angularAcceleration; }
	glm::vec3 getInertia() const { return _inertia; }
	glm::vec3 getInverseInertia() const { return _inverseInertia; }

	//Setters
	void setPos(const glm::vec3& position) { _transform.setPosition(position); }
  void setOrientation(const glm::quat& orientation) { _transform.setRotation(orientation); }
	void setTransform(const Transform& transform) { _transform = transform;}
	void setVelocity(const glm::vec3& velocity) { _velocity = velocity; }
	void setAngularVelocity(const glm::vec3& angularVelocity) { _angularVelocity = angularVelocity; }
	void setTorque(const glm::vec3& torque) { _torque = torque; }
	void setAngularAcceleration(const glm::vec3& angularAcceleration) { _angularAcceleration = angularAcceleration; }
	void setInertia(const glm::vec3& inertia)
	{
		_inertia = inertia;
		_inverseInertia.x = (inertia.x > 0.0f) ? 1.0f / inertia.x : 0.0f;
		_inverseInertia.y = (inertia.y > 0.0f) ? 1.0f / inertia.y : 0.0f;
		_inverseInertia.z = (inertia.z > 0.0f) ? 1.0f / inertia.z : 0.0f;
	}
};

