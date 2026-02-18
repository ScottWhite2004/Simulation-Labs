#pragma once
#include "Scenario.h"
class Application
{
	Scenario* currentScenario;

public:

	Application();

	~Application();
	
	void changeScenario(const Scenario& newScenario);

	void update();

	void draw();

};

