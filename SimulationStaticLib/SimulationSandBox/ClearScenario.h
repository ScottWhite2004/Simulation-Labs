#pragma once
#include "Scenario.h"
class ClearScenario : public Scenario
{

	ImVec4* _colour;
public:
	ClearScenario(ImVec4* colour) : _colour(colour) {}
	~ClearScenario() override;
	void OnLoad() override;
	void OnUpdate(float seconds) override;
	void OnUnload() override;
	void ImGuiMain() override;
};

