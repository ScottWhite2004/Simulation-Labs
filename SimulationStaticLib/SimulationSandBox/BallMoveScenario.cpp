#include "BallMoveScenario.h"

BallMoveScenario::BallMoveScenario()
{
}

BallMoveScenario::~BallMoveScenario()
{
}

void BallMoveScenario::OnLoad()
{
	Sphere sphere(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);

    PhysicsObject moving(
        glm::vec3(-10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(5.0f, 0.0f, 0.0f), // velocity towards origin
        sphere,
        1.0f);

    PhysicsObject stationary(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        sphere,
        1.0f);

	addPhysicsObject(moving);
    addPhysicsObject(stationary);
}

void BallMoveScenario::OnUpdate(float seconds)
{
    // 1. Integrate motion
    for (auto& obj : _PhysicObjects)
    {
        obj.IntegrateEuler(seconds);      // or obj.IntegrateEuler / SemiImplicitEuler
    }

    // 2. Sphere–sphere collisions
    for (size_t i = 0; i < _PhysicObjects.size(); ++i)
    {
        for (size_t j = i + 1; j < _PhysicObjects.size(); ++j)
        {
            Sphere& a = _PhysicObjects[i].GetSphere();
            Sphere& b = _PhysicObjects[j].GetSphere();

            if (a.CollidesWith(b))
            {
                _PhysicObjects[i].SetVelocity(glm::vec3(0.0f));
                _PhysicObjects[j].SetVelocity(glm::vec3(0.0f));
            }
        }
    }

    // 3. Sphere–plane collisions
    for (auto& obj : _PhysicObjects)
    {
        Sphere& s = obj.GetSphere();

        for (const auto& plane : _Planes)
        {
            if (s.CollidesWith(plane))
            {
                obj.SetVelocity(glm::vec3(0.0f));
                break;  // no need to check more planes for this object
            }
        }
    }
}

void BallMoveScenario::OnUnload()
{
	_PhysicObjects.clear();
	_Planes.clear();
}

void BallMoveScenario::ImGuiMain()
{
}





