#pragma once

class Scenario
{

public:
	Scenario() = default;
	virtual ~Scenario();

	virtual void OnLoad();
	virtual void OnUpdate(float seconds);
	virtual void OnUnload();
	virtual void ImGuiMain();

};