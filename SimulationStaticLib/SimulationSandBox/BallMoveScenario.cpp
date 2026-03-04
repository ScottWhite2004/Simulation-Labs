#include "BallMoveScenario.h"

BallMoveScenario::BallMoveScenario()
{
}

BallMoveScenario::~BallMoveScenario()
{
}

void BallMoveScenario::OnLoad()
{
	
}

void BallMoveScenario::OnUpdate(float seconds)
{
	StepSimulation(seconds);
}

void BallMoveScenario::OnUnload()
{
	_PhysicObjects.clear();
}

void BallMoveScenario::ImGuiMain()
{
}

void BallMoveScenario::IntegrateEuler(PhysicsObject& obj, float seconds)
{
	glm::vec3 acceleration = _Gravity * obj.getInverseMass(); // Assuming only gravity for this scenario
	
	glm::vec3 pos = obj.getPos();
	glm::vec3 vel = obj.getVel();
	pos += vel * seconds;
	vel += acceleration * seconds;
	obj.SetPosition(pos);
	obj.SetVelocity(vel);
}

void BallMoveScenario::IntegrateSemiImplicitEuler(PhysicsObject& obj, float seconds)
{
	glm::vec3 acceleration = _Gravity * obj.getInverseMass(); // Assuming only gravity for this scenario
	glm::vec3 pos = obj.getPos();
	glm::vec3 vel = obj.getVel();
	vel += acceleration * seconds;
	pos += vel * seconds;
	obj.SetPosition(pos);
	obj.SetVelocity(vel);
}

void BallMoveScenario::StepSimulation(float seconds)
{
	for (auto& obj : _PhysicObjects)
	{
		switch (obj._selectedIntegrationMethod)
		{
		case PhysicsObject::IntegrationMethod::Euler:
			IntegrateEuler(obj, seconds);
			break;
		case PhysicsObject::IntegrationMethod::SemiImplicitEuler:
			IntegrateSemiImplicitEuler(obj, seconds);
			break;
		default:
			break;
		}
	}
}


