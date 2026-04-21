#pragma once
#include "PhysicsObject.h"
#include "Spring.h"
#include <vector>
class SoftBody : public PhysicsObject
{
	std::vector<Spring> _springs;

public:
	void IntegrateEuler(float seconds) override;
	void IntegrateSemiImplicitEuler(float seconds) override;
};

