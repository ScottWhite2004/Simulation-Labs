#pragma once

class Scenario
{

public:
	Scenario();
	virtual ~Scenario();

	virtual void OnLoad();
	virtual void OnUpdate();
	virtual void OnUnload();
	virtual void ImGuiMain();

};