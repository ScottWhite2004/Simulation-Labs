#pragma once
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

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