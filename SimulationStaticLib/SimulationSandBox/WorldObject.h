#pragma once
#include "glm/glm.hpp"
#include "Shape.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Transform.h"
#include <string>
class WorldObject
{
	std::string _name;
	Collider* _collider{ nullptr };
	RigidBody* _rigidBody{ nullptr };
	Shape* _shape{ nullptr };

	Transform _currentTransform;
	Transform _previousTransform;
	bool _hasInterpolationState = false;

public:
	WorldObject(const std::string& name) : _name(name) {}
	~WorldObject() = default;
	Collider* getCollider() const { return _collider; }
	RigidBody* getRigidBody() const { return _rigidBody; }
	Shape* getShape() const { return _shape; }
	const std::string& getName() const { return _name; }
	void addCollider(Collider* collider) { _collider = collider; }
	void addRigidBody(RigidBody* rigidBody) { _rigidBody = rigidBody; }
	void addShape(Shape* shape) { _shape = shape; }
	void syncTransform() {
		if (_rigidBody && _shape) {
			_shape->setPosition(_rigidBody->getPos());
			_shape->setRotation(_rigidBody->getOrientation());

			if (!_hasInterpolationState)
			{
				_currentTransform = _rigidBody->getTransform();
				_previousTransform = _currentTransform;
				_hasInterpolationState = true;
			}
		}
	}

	void capturePhysicsState()
	{
		if (!_rigidBody)
		{
			return;
		}

		if (!_hasInterpolationState)
		{
			_previousTransform = _rigidBody->getTransform();
			_hasInterpolationState = true;
		}
		else
		{
			_previousTransform = _currentTransform;
			_currentTransform = _rigidBody->getTransform();
		}
	}

	void applyInterpolatedTransform(float alpha)
	{
		if (!_shape)
		{
			return;
		}

		if(!_hasInterpolationState)
		{
			syncTransform();
			return;
		}

		const glm::vec3 interpolatedPos = glm::mix(_previousTransform.getPosition(), _currentTransform.getPosition(), alpha);
		const glm::quat interpolatedRot = glm::slerp(_previousTransform.getRotation(), _currentTransform.getRotation(), alpha);
		const glm::vec3 interpolatedScale = glm::mix(_previousTransform.getScale(), _currentTransform.getScale(), alpha);
		_shape->setPosition(interpolatedPos);
		_shape->setRotation(interpolatedRot);
		_shape->setScale(interpolatedScale);
	}
};

