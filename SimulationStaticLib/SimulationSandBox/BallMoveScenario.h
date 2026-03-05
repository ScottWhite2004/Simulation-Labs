#pragma once
#include "Scenario.h"
#include "PhysicsObject.h"
#include "Plane.h"
#include <vector>
class BallMoveScenario : public Scenario
{

	std::vector<PhysicsObject> _PhysicObjects;
	std::vector<Plane> _Planes;
	const glm::vec3 _Gravity{ 0.0f, -9.81f, 0.0f };

public:
	BallMoveScenario();
	~BallMoveScenario() override;

	void OnLoad() override;

	void OnUpdate(float seconds) override;

	void OnUnload() override;

	void ImGuiMain() override;

	void addPhysicsObject(const PhysicsObject& obj) { _PhysicObjects.push_back(obj); }
	void AddPlane(const Plane& plane) { _Planes.push_back(plane); }

};

