#pragma once
#include "Scenario.h"
#include "PhysicsObject.h"
#include <vector>
class AngularDisplacement : public Scenario
{
	std::vector<PhysicsObject> _PhysicObjects;

public:
	AngularDisplacement() = default;
	~AngularDisplacement() override;
	void OnLoad() override;
	void OnUpdate(float seconds) override;
	void OnUnload() override;
	void ImGuiMain() override;
};

