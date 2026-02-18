#include "Application.h"

Application::Application()
{
	currentScenario = nullptr;
}

Application::~Application()
{
	if (currentScenario)
		currentScenario->OnUnload();
}

void Application::changeScenario(const Scenario& newScenario)
{
	if (currentScenario)
		currentScenario->OnUnload();
	currentScenario = const_cast<Scenario*>(&newScenario);
	currentScenario->OnLoad();
}

void Application::update()
{
	if (currentScenario)
		currentScenario->OnUpdate();
}
