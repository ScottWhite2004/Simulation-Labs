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
	for (auto& obj : _PhysicObjects)
	{
		glm::vec3 pos = obj.getPos();
		pos.x += seconds;
		obj.SetPosition(pos);
	}
}

void BallMoveScenario::OnUnload()
{
}

void BallMoveScenario::ImGuiMain()
{
}


