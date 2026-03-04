#pragma once
#include "Scenario.h"
#include "PhysicsObject.h"
#include <vector>
class BallMoveScenario : public Scenario
{

	std::vector<PhysicsObject> _PhysicObjects;
	const glm::vec3 _Gravity{ 0.0f, -9.81f, 0.0f };

public:
	BallMoveScenario();
	~BallMoveScenario() override;

	void OnLoad() override;

	void OnUpdate(float seconds) override;

	void OnUnload() override;

	void ImGuiMain() override;


private:
	void IntegrateEuler(PhysicsObject& obj, float seconds);
	void IntegrateSemiImplicitEuler(PhysicsObject& obj, float seconds);
	void StepSimulation(float seconds);

};

