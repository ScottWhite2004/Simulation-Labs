#pragma once
#include "Scenario.h"
#include "PhysicsObject.h"
#include <vector>
class AngularVelocity : public Scenario
{
	std::vector<PhysicsObject> _PhysicObjects;

public:
	AngularVelocity() = default;
	~AngularVelocity() override;
	void OnLoad() override;
	void OnUpdate(float seconds) override;
	void OnUnload() override;
	void ImGuiMain() override;
};

