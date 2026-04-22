#pragma once
#include "glm/glm.hpp"
#include "Shape.h"
#include "RigidBody.h"
#include "Collider.h"
#include <string>
class WorldObject
{
	std::string _name;
	Collider* _collider{ nullptr };
	RigidBody* _rigidBody{ nullptr };
	Shape* _shape{ nullptr };

public:
	WorldObject(const std::string& name) : _name(name) {}
	~WorldObject() = default;
	Collider* getCollider() const { return _collider; }
	RigidBody* getRigidBody() const { return _rigidBody; }
	Shape* getShape() const { return _shape; }
	void addCollider(Collider* collider) { _collider = collider; }
	void addRigidBody(RigidBody* rigidBody) { _rigidBody = rigidBody; }
	void addShape(Shape* shape) { _shape = shape; }
	void syncTransform() {
		if (_rigidBody && _shape) {
			_shape->setPosition(_rigidBody->getPos());
			_shape->setRotation(_rigidBody->getOrientation());
		}
	}
};

