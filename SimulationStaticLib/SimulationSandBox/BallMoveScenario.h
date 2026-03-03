#pragma once
#include "Scenario.h"
#include "PhysicsObject.h"
#include <vector>
class BallMoveScenario : public Scenario
{

	std::vector<PhysicsObject> _PhysicObjects;

public:
	BallMoveScenario();
	~BallMoveScenario() override;

	void OnLoad() override;

	void OnUpdate(float seconds) override;

	void OnUnload() override;

	void ImGuiMain() override;


};

